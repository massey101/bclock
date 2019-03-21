#include <avr/interrupt.h>
#include "async_delay.h"


static async_delay_cb_t func;
static volatile ms_t ms;
static volatile ms_t real_ms;


ISR(TIMER0_COMPA_vect) {
    if (ms == 0) {
        async_delay_cb_t temp_func = func;
        async_delay_stop();
        if (temp_func != 0) {
            temp_func(real_ms);
        }
    }
    ms--;
}


void async_delay_trigger() {
    real_ms = real_ms - ms + 10;
    ms = 10;
}


void async_delay_stop() {
    // Disable the interrupt
    TIMSK0 &= ~_BV(OCIE0A);

    // Disable the per-sample timer completely.
    TCCR0B &= ~(_BV(CS00) | _BV(CS01));
    func = 0;
}


void async_delay_ms(
    ms_t _ms,
    async_delay_cb_t _func
) {
    ms = _ms;
    real_ms = _ms;
    func = _func;

    // Set CTC mode (Clear timer on Compare Match)
    // Have to set OCR1A after, otherwise it gets reset to 0.
    TCCR0A &= ~(_BV(WGM00) | _BV(WGM02));
    TCCR0A |= _BV(WGM01);

    // Prescaler of F_IO/64
    TCCR0B &= ~_BV(CS02);
    TCCR0B |= _BV(CS00) | _BV(CS01);

    // Set the compare register
    // The target is 1000Hz
    OCR0A = (F_CPU / 64) / 1000 - 1;

    // Enable interrupt when TCNT1 == OCR1A
    TIMSK0 |= _BV(OCIE0A);
}
