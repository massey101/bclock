#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <math.h>

#include "pcm_tones.h"

#define PCM_MIDDLE 127
#define PCM_LOW 0
// #define SAMPLE_FREQ_mHz 10000000
// #define REAL_SAMPLE_FREQ_mHz 10121457
#define SAMPLE_FREQ_mHz 10000000
#define REAL_SAMPLE_FREQ_mHz 10121457
#define NUM_TONES 4

enum pcm_mode {
    STOPPED,
    STARTING,
    PLAYING,
    FINISHING,
    FINISHED,
    STOPPING
};

const uint8_t sinewave[64] PROGMEM = {
    0x80, 0x83, 0x86, 0x89, 0x8c, 0x8f, 0x92, 0x95, 0x98, 0x9c, 0x9f, 0xa2,
    0xa5, 0xa8, 0xab, 0xae, 0xb0, 0xb3, 0xb6, 0xb9, 0xbc, 0xbf, 0xc1, 0xc4,
    0xc7, 0xc9, 0xcc, 0xce, 0xd1, 0xd3, 0xd5, 0xd8, 0xda, 0xdc, 0xde, 0xe0,
    0xe2, 0xe4, 0xe6, 0xe8, 0xea, 0xeb, 0xed, 0xef, 0xf0, 0xf2, 0xf3, 0xf4,
    0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfb, 0xfc, 0xfd, 0xfd, 0xfe, 0xfe,
    0xfe, 0xff, 0xff, 0xff
};

uint8_t sine(uint8_t x) {
    if (x < 64) {
        return pgm_read_byte(&sinewave[x]);
    } else if (x < 128) {
        return pgm_read_byte(&sinewave[127 - x]);
    } else if (x < 192) {
        return 0xff - pgm_read_byte(&sinewave[x - 128]);
    } else {
        return 0xff - pgm_read_byte(&sinewave[255 - x]);
    }
}
// const uint8_t sinewave[] PROGMEM = {
//     0x80, 0x83, 0x86, 0x89, 0x8c, 0x8f, 0x92, 0x95, 0x98, 0x9c, 0x9f, 0xa2, 0xa5, 0xa8, 0xab, 0xae, 0xb0, 0xb3, 0xb6, 0xb9, 0xbc, 0xbf, 0xc1, 0xc4, 0xc7, 0xc9, 0xcc, 0xce, 0xd1, 0xd3, 0xd5, 0xd8, 0xda, 0xdc, 0xde, 0xe0, 0xe2, 0xe4, 0xe6, 0xe8, 0xea, 0xeb, 0xed, 0xef, 0xf0, 0xf2, 0xf3, 0xf4, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfb, 0xfc, 0xfd, 0xfd, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfd, 0xfd, 0xfc, 0xfc, 0xfb, 0xfa, 0xf9, 0xf8, 0xf7, 0xf6, 0xf5, 0xf4, 0xf2, 0xf1, 0xef, 0xee, 0xec, 0xeb, 0xe9, 0xe7, 0xe5, 0xe3, 0xe1, 0xdf, 0xdd, 0xdb, 0xd9, 0xd7, 0xd4, 0xd2, 0xcf, 0xcd, 0xca, 0xc8, 0xc5, 0xc3, 0xc0, 0xbd, 0xba, 0xb8, 0xb5, 0xb2, 0xaf, 0xac, 0xa9, 0xa6, 0xa3, 0xa0, 0x9d, 0x9a, 0x97, 0x94, 0x91, 0x8e, 0x8a, 0x87, 0x84, 0x81, 0x7e, 0x7b, 0x78, 0x75, 0x71, 0x6e, 0x6b, 0x68, 0x65, 0x62, 0x5f, 0x5c, 0x59, 0x56, 0x53, 0x50, 0x4d, 0x4a, 0x47, 0x45, 0x42, 0x3f, 0x3c, 0x3a, 0x37, 0x35, 0x32, 0x30, 0x2d, 0x2b, 0x28, 0x26, 0x24, 0x22, 0x20, 0x1e, 0x1c, 0x1a, 0x18, 0x16, 0x14, 0x13, 0x11, 0x10, 0x0e, 0x0d, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x03, 0x02, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x02, 0x02, 0x03, 0x04, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0b, 0x0c, 0x0d, 0x0f, 0x10, 0x12, 0x14, 0x15, 0x17, 0x19, 0x1b, 0x1d, 0x1f, 0x21, 0x23, 0x25, 0x27, 0x2a, 0x2c, 0x2e, 0x31, 0x33, 0x36, 0x38, 0x3b, 0x3e, 0x40, 0x43, 0x46, 0x49, 0x4c, 0x4f, 0x51, 0x54, 0x57, 0x5a, 0x5d, 0x60, 0x63, 0x67, 0x6a, 0x6d, 0x70, 0x73, 0x76, 0x79, 0x7c, 0x7f
// };
// 
// uint8_t sine(uint8_t x) {
//     return pgm_read_byte(&sinewave[x]);
// }


typedef int32_t us_t;

struct tone {
    uint16_t i;
    uint16_t increase;
    uint8_t playing;
    uint8_t volume;
    uint32_t samples;
    uint32_t num_samples;
    pcm_tones_cb_t done;
};

uint8_t tone_next_sample(volatile struct tone *t);

void tone_init(
        volatile struct tone *t,
        mHz_t freq,
        uint8_t volume,
        ms_t duration,
        pcm_tones_cb_t done
) {
    t->i = 0;
    t->increase = ((uint64_t) 0xffff * ( (uint64_t) freq)) / ((uint64_t) REAL_SAMPLE_FREQ_mHz);
    t->playing = 1;
    t->volume = 7 - volume;
    t->num_samples = ((uint32_t) duration) * 10; // (duration * SAMPLE_FREQ_mHz)
    t->samples = 0;
    t->done = done;
    printf("increase: %u, num_samples: %lu\n", t->increase, t->num_samples);
}

// 0xff                .                               .
//                 .       .                       .       .
//               .           .                   .
//              .             .                 .
//             .               .               .
//            .                 .             .
//           .                   .           .
//         .                       .       .
//   0 .                               .
//     0                            0xffff
//     <--------  1 / freq  ----------->

uint8_t tone_next_sample(volatile struct tone *t) {
    uint8_t sample = sine(t->i >> 8);
    t->i = t->i + t->increase;
    t->samples++;
    if (t->samples == t->num_samples) {
        t->playing = 0;
    }

    return sample >> t->volume;
}

static pcm_tones_cb_t done_cb;
static volatile enum pcm_mode current_mode;
static volatile struct tone tones[NUM_TONES];


void stop_playback() {
    current_mode = STOPPED;

    // Disable playback per-sample interrupt.
    TIMSK1 &= ~_BV(OCIE1A);

    // Disable the per-sample timer completely.
    TCCR1B &= ~_BV(CS10);

    // Disable the PWM timer.
    TCCR0B &= ~_BV(CS00);

    PORTD &= ~_BV(PD6);
}


void start_playback() {
    // Set up Timer 1 to send a sample every interrupt.

    cli();

    // Set CTC mode (Clear Timer on Compare Match) (p.133)
    // Have to set OCR1A *after*, otherwise it gets reset to 0!
    TCCR1B &= ~_BV(WGM13);
    TCCR1B |= _BV(WGM12);
    TCCR1A &= ~(_BV(WGM11) | _BV(WGM10));

    // Prescaler of /8
    TCCR1B &= ~(_BV(CS12) | _BV(CS10));
    TCCR1B |= _BV(CS11);

    // Set the compare register (OCR1A).
    // OCR1A is a 16-bit register, so we have to do this with
    // interrupts disabled to be safe.
    OCR1A = F_CPU / (SAMPLE_FREQ_mHz / 1000) / 8;

    // Enable interrupt when TCNT1 == OCR1A (p.136)
    TIMSK1 |= _BV(OCIE1A);

    // Enable the PWM timer.
    TCCR0B |= _BV(CS00);

    current_mode = PLAYING;

    sei();
}

// This is called at 10000 Hz to load the next sample.
// Must run within ~88us as there is around an ~22us buffer between the interrupts.
ISR(TIMER1_COMPA_vect) {
    PORTB |= _BV(PB1);

    uint16_t sum = 0;
    uint8_t num = 0;
    if (tones[0].playing == 1) {
        sum = tone_next_sample(&tones[0]);
        num += 1;
    }

    if (tones[1].playing == 1) {
        sum += tone_next_sample(&tones[1]);
        num += 1;
    }

    if (tones[2].playing == 1) {
        sum += tone_next_sample(&tones[2]);
        num += 1;
    }

    if (tones[3].playing == 1) {
        sum += tone_next_sample(&tones[3]);
        num += 1;
    }

    OCR0A = sum / num;

    if (num == 0) {
        stop_playback();
    }
    PORTB &= ~(_BV(PB1));
}


void pcm_tones_init() {
    // Setup PORTD PIN 6 as an ouput (OC0A)
    DDRD |= _BV(PD6);
    DDRB |= _BV(PB1);

    // Set up Timer 0 to do pulse width modulation on the speaker
    // pin.

    // Set fast PWM mode  (p.157)
    TCCR0A |= _BV(WGM01) | _BV(WGM00);
    TCCR0B &= ~_BV(WGM02);

    // Do non-inverting PWM on pin OC0A (p.155)
    TCCR0A |= _BV(COM0A1);
    TCCR0A &= ~_BV(COM0A0);

    // No prescaler (p.158) (enables timer)
    TCCR0B &= ~(_BV(CS02) | _BV(CS01));
    // Setting TCCR0B |= _BV(CS00); will enable the timer.

    // Set initial pulse width to the first sample.
    OCR0A = PCM_LOW;
};



void pcm_tones_play(
    mHz_t frequency,
    ms_t duration,
    uint8_t volume,
    pcm_tones_cb_t _done_cb
) {
    for (uint8_t i = 0; i < NUM_TONES; i++) {
        if (tones[i].playing != 0) {
            continue;
        }

        tone_init(&tones[i], frequency, volume, duration, _done_cb);
        break;
    }

    if (current_mode != PLAYING) {
        start_playback();
    }
}


void pcm_tones_stop() {
    for (uint8_t i = 0; i < NUM_TONES; i++) {
        tones[i].playing = 0;
    }
}


uint8_t pcm_tones_busy() {
    for (uint8_t i = 0; i < NUM_TONES; i++) {
        if (tones[i].playing == 1) {
            return -1;
        }
    }
    if (current_mode == PLAYING) {
        return -1;
    } else {
        return 0;
    }
};
