#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "buttons.h"


#define BUTTONS_PORT PORTC
#define BUTTONS_PIN PINC
#define BUTTONS_DDR DDRC
#define BUTTONS (_BV(PC0) | _BV(PC1) | _BV(PC2) | _BV(PC3))

buttons_cb_t cb;

char button_to_input(uint8_t button) {
    if (button & (0x01 << 0)) {
        return 'R';
    }

    if (button & (0x01 << 1)) {
        return 'U';
    }

    if (button & (0x01 << 2)) {
        return 'D';
    }

    if (button & (0x01 << 3)) {
        return 'L';
    }

    return '0';
}

static volatile uint8_t last_state = 0;

ISR(PCINT1_vect) {
    uint8_t buttons = ~(BUTTONS_PIN) & BUTTONS;
    if (buttons == last_state) {
        return;
    }
    last_state = buttons;
    if (! buttons) {
        return;
    }

    for (uint8_t i = 0; i < 5; i++) {
        _delay_ms(1);
        uint8_t check_buttons = ~(BUTTONS_PIN) & BUTTONS;
        if (check_buttons != buttons) {
            return;
        }
    }

    if (cb != 0) {
        cb(button_to_input(buttons));
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
}
