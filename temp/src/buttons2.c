#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "reactor.h"
#include "tasks.h"
#include "buttons.h"


#include <stdio.h>

#define BUTTONS_PORT PORTC
#define BUTTONS_PIN PINC
#define BUTTONS_DDR DDRC
#define BUTTONS (_BV(PC0) | _BV(PC1))

buttons_cb_t cb;
uint8_t ct0 = 0xff, ct1 = 0xff;
uint8_t state;

uint8_t key_state;
uint8_t key_press;

// void check_buttons(ms_t real_ms) {
//     uint8_t i;
//     i = key_state ^ ~BUTTONS_PIN;
// 
//     ct0 = ~(ct0 & i);
//     ct1 = ct0 ^ (ct1 & i);
//     i &= ct0 & ct1;
//     key_state ^= i;
//     key_press |= key_state & i;
//     reactor_call_later(TASK_BUTTONS, &check_buttons, 10);
// }

// void check_buttons2(ms_t real_ms) {
//     ct &= ~(BUTTONS_PIN & (BUTTONS));
//     state++;
//     if (state == 4) {
//         char input = '0';
//         if (ct & (0x01 << 0)) {
//             input = '1';
//         }
//         if (ct & (0x01 << 1)) {
//             input = '2';
//         }
// 
//         if (cb != 0) {
//             cb(input);
//         }
// 
//         return;
//     }
// 
//     reactor_call_later(TASK_BUTTONS, &check_buttons, 10);
// }


ISR(PCINT1_vect) {
    printf("T: 0x%02x\n", ~(BUTTONS_PIN) & BUTTONS);
    uint8_t in = ~(BUTTONS_PIN) & BUTTONS;
    if (! in) {
        return;
    }
    for (uint8_t i = 0; i < 4; i++) {
        _delay_ms(10);
        uint8_t in2 = ~(BUTTONS_PIN) & BUTTONS;
        if (in != in2) {
            return;
        }
    }
    char input = '0';
    if (in & (0x01 << 0)) {
        input = '1';
    }
    if (in & (0x01 << 1)) {
        input = '2';
    }

    if (cb != 0) {
        cb(input);
    }
}

void buttons_init(buttons_cb_t _cb) {
    cb = _cb;

    // Enable interrupts on PORTC
    // Pins PCINT8 to PCINT11
    BUTTONS_DDR &= ~(BUTTONS);
    BUTTONS_PORT |= BUTTONS;
    PCICR |= _BV(PCIE1);
    PCMSK1 |= BUTTONS;

    printf("Test1\n");
    // reactor_call_later(TASK_BUTTONS, &check_buttons, 10);
}
