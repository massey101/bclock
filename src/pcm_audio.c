#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "pcm_audio.h"


#define PCM_MIDDLE 127


volatile uint16_t sample_i;
volatile uint8_t first_sample;
volatile uint8_t last_sample;
volatile struct pcm_audio * current_audio;


void stopPlayback() {
    // Disable playback per-sample interrupt.
    TIMSK1 &= ~_BV(OCIE1A);

    // Disable the per-sample timer completely.
    TCCR1B &= ~_BV(CS10);

    current_audio = 0;
}

// This is called at 8000 Hz to load the next sample.
ISR(TIMER1_COMPA_vect) {
    if (sample_i >= current_audio->length) {
        // When we have finished ramp down the audio before stopping.
        if (last_sample == PCM_MIDDLE) {
            stopPlayback();
            return;
        }
        if (last_sample > PCM_MIDDLE) {
            last_sample--;
        } else if (last_sample < PCM_MIDDLE) {
            last_sample++;
        }

        OCR2B = last_sample;
        return;
    }

    uint8_t sample = pgm_read_byte(&current_audio->data[sample_i]);
    if (sample_i == 0 && first_sample != sample) {
        if (first_sample > sample) {
            first_sample--;
        } else if (first_sample < sample) {
            first_sample++;
        }

        OCR2B = first_sample;

        return;
    }

    OCR2B = sample;
    sample_i++;
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
    TCCR2B |= _BV(CS10);

    // Set initial pulse width to the first sample.
    OCR2B = PCM_MIDDLE;
};


void pcm_audio_play(struct pcm_audio * pcm_audio) {
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

    sample_i = 0;
    first_sample = PCM_MIDDLE;
    last_sample = pgm_read_byte(&pcm_audio->data[pcm_audio->length-1]);
    current_audio = pcm_audio;
    sei();
}

uint8_t pcm_audio_busy() {
    if (current_audio) {
        return -1;
    } else {
        return 0;
    }
};
