#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "async_delay.h"

#define OSC_FREQ 32768


static async_delay_cb_t func;
static volatile ms_t ms;
static volatile ms_t real_ms;


ISR(TIMER2_COMPA_vect) {
    if (ms == 0) {
        async_delay_cb_t temp_func = func;
        async_delay_stop();
        if (temp_func != 0) {
            temp_func(real_ms);
        }
    }
    ms--;

    // set_sleep_mode(SLEEP_MODE_PWR_SAVE);
    // sleep_mode();
}


void async_delay_trigger() {
    real_ms = real_ms - ms + 10;
    ms = 10;
}


void async_delay_stop() {
    // Disable the interrupt
    TIMSK2 &= ~_BV(OCIE2A);

    // Disable the per-sample timer completely.
    TCCR2B &= ~_BV(CS22) & ~_BV(CS21) & ~_BV(CS20);
    func = 0;
}


void async_delay_ms(
    ms_t _ms,
    async_delay_cb_t _func
) {
    ms = _ms;
    real_ms = _ms;
    func = _func;

    // User external 32.768kHz oscillator
    ASSR &= ~_BV(EXCLK);
    ASSR &= _BV(AS2);

    // Set CTC mode (Clear timer on Compare Match)
    // Have to set OCR2A after, otherwise it gets reset to 0.
    TCCR2A &= ~_BV(WGM22) & ~_BV(WGM20);
    TCCR2A |= _BV(WGM21);

    // Prescaler of 1
    TCCR2B &= ~_BV(CS22) & ~_BV(CS21);
    TCCR2B |= _BV(CS20);

    // Set the compare register
    // The target is 1000Hz
    OCR2A = 1 * OSC_FREQ / 1000 - 1;

    // Enable interrupt when TCNT2 == OCR2A
    TIMSK2 |= _BV(OCIE2A);

    // set_sleep_mode(SLEEP_MODE_PWR_SAVE);
    // sleep_mode();
}
