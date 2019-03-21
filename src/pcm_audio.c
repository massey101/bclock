#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "pcm_audio.h"


#define PCM_MIDDLE 127
#define PCM_LOW 0

enum pcm_mode {
    STOPPED,
    STARTING,
    PLAYING,
    FINISHING,
    FINISHED,
    STOPPING
};

static volatile uint16_t sample_i;
static volatile uint8_t current_sample;
static volatile uint8_t first_sample;
static const struct pcm_audio * volatile current_audio;
static pcm_audio_cb_t done_cb;
static volatile enum pcm_mode current_mode;


uint8_t pcm_audio_get_sample(uint16_t i) {
    return pgm_read_byte(&current_audio->data[i]);
}


void stop_playback() {
    // Disable playback per-sample interrupt.
    TIMSK1 &= ~_BV(OCIE1A);

    // Disable the per-sample timer completely.
    TCCR1B &= ~_BV(CS10);

    // Disable the PWM timer.
    // TCCR0B &= ~_BV(CS00);

    PORTD &= ~_BV(PD6);
}


// This is called at 8000 Hz to load the next sample.
ISR(TIMER1_COMPA_vect) {
    if (current_mode == STARTING) {
        // Ramp up/down to the value of the first sample. Once there start
        // playing immediately.
        if (current_sample > first_sample) {
            current_sample--;
        } else if (current_sample < first_sample) {
            current_sample++;
        }
        OCR0A = current_sample;

        if (current_sample == first_sample) {
            current_mode = PLAYING;
        }
    }

    if (current_mode == PLAYING) {
        // Move through the file and play each sample. Once we reach the end
        // start pausing and notify the caller that we have finished.
        current_sample = pcm_audio_get_sample(sample_i);
        OCR0A = current_sample;
        sample_i++;

        if (sample_i >= current_audio->length) {
            current_mode = FINISHING;
            current_audio = 0;
            if (done_cb != 0) {
                done_cb();
            }
        }
    }

    if (current_mode == FINISHING) {
        // Ramp up/down to the middle value. Once there consider yourself
        // paused.
        if (current_sample > PCM_MIDDLE) {
            current_sample--;
        } else if (current_sample < PCM_MIDDLE) {
            current_sample++;
        }

        OCR0A = current_sample;

        if (current_sample == PCM_MIDDLE) {
            current_mode = FINISHED;
        }
    }

    if (current_mode == FINISHED) {
        // Do nothing
    }

    if (current_mode == STOPPING) {
        // Ramp up/down to the middle value. Once there consider yourself
        // paused.
        if (current_sample > PCM_LOW) {
            current_sample--;
        } else if (current_sample < PCM_LOW) {
            current_sample++;
        }

        OCR0A = current_sample;

        if (current_sample == PCM_LOW) {
            current_mode = STOPPED;
        }
    }

    if (current_mode == STOPPED) {
        stop_playback();
    }
}


void pcm_audio_init() {
    // Setup PORTD PIN 6 as an ouput (OC0A)
    DDRD |= _BV(PD6);

    // Set up Timer 0 to do pulse width modulation on the speaker
    // pin.

    // Set fast PWM mode  (p.157)
    TCCR0A |= _BV(WGM01) | _BV(WGM00);
    TCCR0B &= ~_BV(WGM02);

    // Do non-inverting PWM on pin OC0A (p.155)
    TCCR0A |= _BV(COM0B1);
    TCCR0A &= ~_BV(COM0B0);

    // No prescaler (p.158) (enables timer)
    TCCR0B &= ~_BV(CS02) & ~_BV(CS01);
    TCCR0B |= _BV(CS00);

    // Set initial pulse width to the first sample.
    OCR0A = PCM_LOW;
};


void pcm_audio_play(
    const struct pcm_audio * pcm_audio,
    pcm_audio_cb_t _done_cb
) {
    // Set up Timer 1 to send a sample every interrupt.

    cli();

    // Set CTC mode (Clear Timer on Compare Match) (p.133)
    // Have to set OCR1A *after*, otherwise it gets reset to 0!
    TCCR1B &= ~_BV(WGM13);
    TCCR1B |= _BV(WGM12);
    TCCR1A &= ~(_BV(WGM11) | _BV(WGM10));

    // No prescaler (p.134)
    TCCR1B &= ~(_BV(CS12) | _BV(CS11));
    TCCR1B |= _BV(CS10);

    // Set the compare register (OCR1A).
    // OCR1A is a 16-bit register, so we have to do this with
    // interrupts disabled to be safe.
    OCR1A = F_CPU / pcm_audio->sample_rate; // 16e6 / 8000 = 2000

    // Enable interrupt when TCNT1 == OCR1A (p.136)
    TIMSK1 |= _BV(OCIE1A);

    // Enable the PWM timer.
    TCCR0A |= _BV(CS00);

    sample_i = 0;
    current_audio = pcm_audio;
    first_sample = pcm_audio_get_sample(0);
    done_cb = _done_cb;
    current_mode = STARTING;
    sei();
}


void pcm_audio_stop() {
    current_mode = STOPPING;
}


uint8_t pcm_audio_busy() {
    if (current_audio) {
        return -1;
    } else {
        return 0;
    }
};
