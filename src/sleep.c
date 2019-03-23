#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include "uart.h"
#include "sleep.h"

#define OSC_FREQ 8000000

// Used to verify that the interrupt that woke us up was actually out sleep
// timer TIMER2.
static volatile uint8_t ticked;

// Used to indicate that we should abort and not go back to sleep.
static volatile uint8_t wakeup;

// Used to indicate that we should enter power save mode. The application may
// need to disable this temporarily to do thing like play audio.
static volatile uint8_t power_save_enabled = 1;


ISR(TIMER2_COMPA_vect) {
    ticked = 1;
}


void sleep_wakeup() {
    wakeup = 1;
}


void sleep_power_save_disable() {
    power_save_enabled = 0;
}


void sleep_power_save_enable() {
    power_save_enabled = 1;
}


void stop_timer() {
    // Disable the interrupt
    TIMSK2 &= ~_BV(OCIE2A);

    // Disable the per-sample timer completely.
    TCCR2B &= ~_BV(CS22) & ~_BV(CS21) & ~_BV(CS20);
}


void init_timer() {

    // Use internal oscillator
    ASSR &= ~_BV(EXCLK) & ~_BV(AS2);

    // Set CTC mode (Clear timer on Compare Match)
    // Have to set OCR2A after, otherwise it gets reset to 0.
    TCCR2A &= ~_BV(WGM22) & ~_BV(WGM20);
    TCCR2A |= _BV(WGM21);

    // Prescaler of 32x
    TCCR2B &= ~_BV(CS22);
    TCCR2B |= _BV(CS21) | _BV(CS20);

    // Set the compare register
    // The target is 1000Hz
    OCR2A = 1 * (OSC_FREQ / 32) / 1000 - 1;

    // Enable interrupt when TCNT2 == OCR2A
    TIMSK2 |= _BV(OCIE2A);
    sei();
}


ms_t sleep_ms(ms_t ms) {
    ms_t target_ms = ms;

    // This is used to ensure the uart is clear before we enter sleep as
    // otherwise we could get garbage out.
    uart_flush();

    init_timer();

    ticked = 0;
    wakeup = 0;

    // Sample sleep_enabled once to avoid a race condidiont
    // Prepare to enter sleep mode
    uint8_t _power_save_enabled = power_save_enabled;
    if (_power_save_enabled) {
        set_sleep_mode(SLEEP_MODE_PWR_SAVE);
        sleep_enable();
    }

    while (ms) {
        if (_power_save_enabled ) {
            sleep_cpu();
        }

        // Someone has requested the sleep to end.
        if (wakeup) {
            if (ticked) {
                ms--;
            }
            break;
        }

        // Another interrupt has rudely awoken us. Ignore it.
        if (! ticked) {
            continue;
        }

        ticked = 0;
        ms--;
    }
    if (_power_save_enabled ) {
        sleep_disable();
    }

    stop_timer();

    return target_ms - ms;
}
