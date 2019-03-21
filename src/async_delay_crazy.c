#include <avr/interrupt.h>
#include "async_delay.h"

#define OSC_FREQ 32768


static async_delay_cb_t func;
static volatile uint32_t clocks;
static volatile uint32_t clocks_skipped;
static volatile uint16_t prescaler;
static volatile uint32_t repeats;
static volatile uint8_t remainder;
static volatile uint32_t ms;
static volatile uint32_t real_ms;

static const uint16_t prescalers[] = {
    1,
    8,
    32,
    64,
    128,
    256,
    1024,
};


uint32_t calculate_real_ms() {
    // Repeats gives us the number of repeats that would be remaining.
    uint32_t clocks_done = clocks - clocks_skipped;
    uint32_t ms_done = (((clocks_done + 1) * 1000 * prescalers[prescaler]) / OSC_FREQ);
    return ms_done;
}


void load_output_compare() {
    if (repeats) {
        OCR2A = 256 - 1;
        repeats--;
    } else {
        OCR2A = remainder;
        remainder = 0;
    }
}


ISR(TIMER2_COMPA_vect) {
    if (repeats == 0 && remainder == 0) {
        async_delay_cb_t temp_func = func;
        async_delay_stop();
        if (temp_func != 0) {
            uint32_t real_ms = calculate_real_ms();
            temp_func(real_ms);
        }

        return;
    }

    load_output_compare();
}


void async_delay_trigger() {
    clocks_skipped = (256 * repeats) + remainder + (OCR0A - TCNT2) - 1;

    repeats = 0;
    remainder = 0;
    OCR0A = 1;
}


void async_delay_stop() {
    // Disable the interrupt
    TIMSK2 &= ~_BV(OCIE2A);

    // Disable the per-sample timer completely.
    TCCR2B &= ~_BV(CS22) & ~_BV(CS21) & ~_BV(CS20);
    func = 0;
}


void calculate_prescaler(uint32_t ms) {
    uint32_t clocks = 0;
    for (uint8_t i = 0; i < sizeof(prescalers) / sizeof(prescalers[0]); i++) {
        prescaler = i;
        clocks = ms * (OSC_FREQ / prescalers[i]) / 1000 - 1;
        if (clocks < 256) {
            break;
        }
    }

    repeats = (clocks / 256) - 1;
    remainder = clocks % 256;
}


void async_delay_ms(
    uint32_t _ms,
    async_delay_cb_t _func
) {
    func = _func;
    clocks_skipped = 0;

    calculate_prescaler(ms);


    // Use external 32.768kHz oscillator
    ASSR &= ~_BV(EXCLK);
    ASSR |= _BV(AS2);

    // Set CTC mode (Clear timer on Compare Match)
    // Have to set OCR2A after, otherwise it gets reset to 0.
    TCCR2A &= ~_BV(WGM22) & ~_BV(WGM20);
    TCCR2A |= _BV(WGM21);

    // Prescaler as determined by clock
    TCCR2B &= ~_BV(CS22) & ~_BV(CS21) & ~_BV(CS20);
    TCCR2B |= (prescaler & 0x7) << CS00;

    // Set the compare register
    load_output_compare();

    // Enable interrupt when TCNT2 == OCR2A
    TIMSK2 |= _BV(OCIE2A);
}
