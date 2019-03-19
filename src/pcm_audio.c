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
static void (* volatile done_cb)(volatile void * volatile ctx);
static volatile void * volatile done_cb_ctx;
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
    // TCCR2B &= ~_BV(CS10);

    PORTD &= ~_BV(PD3);
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
        OCR2B = current_sample;

        if (current_sample == first_sample) {
            current_mode = PLAYING;
        }
    }

    if (current_mode == PLAYING) {
        // Move through the file and play each sample. Once we reach the end
        // start pausing and notify the caller that we have finished.
        current_sample = pcm_audio_get_sample(sample_i);
        OCR2B = current_sample;
        sample_i++;

        if (sample_i >= current_audio->length) {
            current_mode = FINISHING;
            current_audio = 0;
            if (done_cb != 0) {
                done_cb(done_cb_ctx);
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

        OCR2B = current_sample;

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

        OCR2B = current_sample;

        if (current_sample == PCM_LOW) {
            current_mode = STOPPED;
        }
    }

    if (current_mode == STOPPED) {
        stop_playback();
    }
}


void pcm_audio_init() {
    // Setup PORTD PIN 3 as an ouput (OC2B)
    DDRD |= _BV(PD3);

    // Set up Timer 2 to do pulse width modulation on the speaker
    // pin.

    // Use internal clock (datasheet p.160)
    ASSR &= ~(_BV(EXCLK) | _BV(AS2));

    // Set fast PWM mode  (p.157)
    TCCR2A |= _BV(WGM21) | _BV(WGM20);
    TCCR2B &= ~_BV(WGM22);

    // Do non-inverting PWM on pin OC2B (p.155)
    // On the Arduino this is pin 3.
    TCCR2A |= _BV(COM2B1);
    TCCR2A &= ~_BV(COM2B0);
    // No prescaler (p.158)
    TCCR2B &= ~(_BV(CS12) | _BV(CS11));
    // This will enable the PWM timer
    TCCR2B |= _BV(CS10);

    // Set initial pulse width to the first sample.
    OCR2B = PCM_LOW;
};


void pcm_audio_play(
    const struct pcm_audio * pcm_audio,
    void (* volatile _done_cb)(volatile void * volatile ctx),
    volatile void * volatile _done_cb_ctx
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
    TCCR2B |= _BV(CS10);

    sample_i = 0;
    current_audio = pcm_audio;
    first_sample = pcm_audio_get_sample(0);
    done_cb = _done_cb;
    done_cb_ctx = _done_cb_ctx;
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
