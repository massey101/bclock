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

uint8_t last_key_state = 0x00;
uint8_t key_state;
uint8_t key_press;

uint8_t counter = 0;

char button_to_input(uint8_t button) {
    if (button & (0x01 << 0)) {
        return '1';
    }

    if (button & (0x01 << 1)) {
        return '2';
    }

    return '0';
}


uint8_t buttons;

void check_buttons3(ms_t real_ms) {

    uint8_t buttons2 = ~(BUTTONS_PIN) & BUTTONS;
    if (buttons != buttons2) {
        return;
    }

    counter--;
    if (counter == 0) {
        if (cb != 0) {
            cb(button_to_input(buttons));
            return;
        }
    }

    reactor_call_later(TASK_BUTTONS, &check_buttons3, 1);
    reactor_update();
}

ISR(PCINT1_vect) {
    counter = 4;
    buttons = ~(BUTTONS_PIN) & BUTTONS;
    if (! buttons) {
        reactor_cancel(TASK_BUTTONS);
        return;
    }

    reactor_cancel(TASK_BUTTONS);
    reactor_call_later(TASK_BUTTONS, &check_buttons3, 1);
    reactor_update();
}

void buttons_init(buttons_cb_t _cb) {
    cb = _cb;

    // Enable interrupts on PORTC
    // Pins PCINT8 to PCINT11
    BUTTONS_DDR &= ~(BUTTONS);
    BUTTONS_PORT |= BUTTONS;
    PCICR |= _BV(PCIE1);
    PCMSK1 |= BUTTONS;

    key_state = ~BUTTONS_PIN;
}
