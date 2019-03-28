/**
 *  @filename   :   epdif.cpp
 *  @brief      :   Implements EPD interface functions
 *                  Users have to implement all the functions in epdif.cpp
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     August 10 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <util/delay.h>
#include <avr/interrupt.h>

#include "epdif.h"

// CS = PB2
// RESET = PD7
// DC = PD3
// BUSY = PD5

static epdif_busy_cb_t busy_cb = 0;


void epd_if_DigitalWrite(int pin, int value) {
    if (pin == RST_PIN) {
        if (value == LOW) {
            PORTD &= ~(1<<PD7);
        } else {
            PORTD |= (1<<PD7);
        }
    }
    if (pin == DC_PIN) {
        if (value == LOW) {
            PORTD &= ~(1<<PD3);
        } else {
            PORTD |= (1<<PD3);
        }
    }
    if (pin == CS_PIN) {
        if (value == LOW) {
            PORTB &= ~(1<<PB2);
        } else {
            PORTB |= (1<<PB2);
        }
    }
    if (pin == BUSY_PIN) {
        if (value == LOW) {
            PORTD &= ~(1<<PD5);
        } else {
            PORTD |= (1<<PD5);
        }
    }
}


int epd_if_DigitalRead(int pin) {
    if (pin == RST_PIN) {
        return (PIND & (1<<PD7)) >> PD7;
    }
    if (pin == DC_PIN) {
        return (PIND & (1<<PD3)) >> PD3;
    }
    if (pin == CS_PIN) {
        return (PINB & (1<<PB2)) >> PB2;
    }
    if (pin == BUSY_PIN) {
        return (PIND & (1<<PD5)) >> PD5;
    }
    return -1;
}


void epd_if_SpiTransfer(unsigned char data) {
    epd_if_DigitalWrite(CS_PIN, LOW);
    SPDR = data;
    while (!(SPSR & (1<<SPIF)));
    epd_if_DigitalWrite(CS_PIN, HIGH);
}


int epd_if_Init() {
    DDRB |= (1<<PB3) | (1<<PB5) | (1<<PB2);
    DDRD |= (1<<PD7) | (1<<PD3);
    DDRD &= ~(1<<PD5);
    SPCR |= (1<<SPE) | (1<<MSTR);
    SPSR |= (1<<SPI2X);
    PORTB |= (1<<PB2);
    return 0;
}


ISR(PCINT2_vect) {
    if (busy_cb) {
        int value = epd_if_DigitalRead(BUSY_PIN);
        epdif_busy_cb_t temp_busy_cb = busy_cb;
        busy_cb = 0;
        if (temp_busy_cb(value)) {
            epd_if_disable_busy_interrupt();
        }
    }
}


void epd_if_enable_busy_interrupt(epdif_busy_cb_t _busy_cb) {
    busy_cb = _busy_cb;

    // The reset pin is on pin D7 which corrosponds to PCINT21
    PCICR |= (1<<PCIE2);
    PCMSK2 |= (1<<PCINT21);
    sei();
}

void epd_if_disable_busy_interrupt() {
    PCMSK2 &= ~(1<<PCINT21);
    PCICR &= ~(1<<PCIE2);
    busy_cb = 0;
}
