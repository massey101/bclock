#include <avr/interrupt.h>
#include "async_delay.h"


static cb_t func;
static pctx_t func_ctx;
static volatile uint16_t ms;


ISR(TIMER0_COMPA_vect) {
    if (ms == 0) {
        cb_t temp_func = func;
        pctx_t temp_func_ctx = func_ctx;
        async_delay_stop();
        if (temp_func != 0) {
            temp_func(temp_func_ctx);
        }
    }
    ms--;
}


void async_delay_trigger() {
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
    uint16_t _ms,
    cb_t _func,
    pctx_t _func_ctx
) {
    ms = _ms;
    func_ctx = _func_ctx;
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
