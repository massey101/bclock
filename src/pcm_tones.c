#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <math.h>

#include "pcm_tones.h"

#define PCM_MIDDLE 127
#define PCM_LOW 0
#define SAMPLE_FREQ_mHz 10000000
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

typedef int32_t us_t;

struct tone {
    uint16_t i;
    uint32_t increase;
    mHz_t freq;
    uint8_t playing;
    uint8_t volume;
    us_t duration;
    us_t playtime;
    us_t playtime_increase;
    pcm_tones_cb_t done;
};

void tone_init(
        volatile struct tone *t,
        mHz_t freq,
        uint8_t volume,
        ms_t duration,
        pcm_tones_cb_t done
) {
    t->i = 0;
    t->increase = ((uint64_t) 0xffff * ( (uint64_t) freq)) / ((uint64_t) SAMPLE_FREQ_mHz);
    t->freq = freq;
    t->playing = 1;
    t->volume = volume;
    t->duration = 1000 * duration;
    t->playtime = 0;
    t->playtime_increase = 1000000 / (SAMPLE_FREQ_mHz / 1000);
    t->done = done;
    printf("increase = %lu\n", (long unsigned int) t->increase);
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

    t->playtime += t->playtime_increase;
    if (t->playtime > t->duration) {
        t->playing = 0;
        if (t->done != 0) {
            t->done();
        }
    }

    int16_t volume_applied = (sample - 127) * t->volume / 0xff + 127;

    return volume_applied;
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

    // No prescaler (p.134)
    // TCCR1B &= ~(_BV(CS12) | _BV(CS11));
    // TCCR1B |= _BV(CS10);
    // Prescaler of /8
    TCCR1B &= ~(_BV(CS12) | _BV(CS10));
    TCCR1B |= _BV(CS11);

    // Set the compare register (OCR1A).
    // OCR1A is a 16-bit register, so we have to do this with
    // interrupts disabled to be safe.
    OCR1A = F_CPU / (SAMPLE_FREQ_mHz / 1000) / 8; // 16e6 / 10000 = 1600

    // Enable interrupt when TCNT1 == OCR1A (p.136)
    TIMSK1 |= _BV(OCIE1A);

    // Enable the PWM timer.
    TCCR0B |= _BV(CS00);

    current_mode = PLAYING;

    sei();
}

// This is called at 10000 Hz to load the next sample.
ISR(TIMER1_COMPA_vect) {
    int16_t sum = 0;
    int8_t num = 0;
    for (uint8_t i = 0; i < NUM_TONES; i++) {
        if (tones[i].playing == 1) {
            sum += (tone_next_sample(&tones[i]) - 127);
            num += 1;
        }
    }

    // TODO: Multiple tones don't seem to mix properly. Unsure what the issue is.
    if (num > 0) {
        OCR0A = (sum / num) + 127;
    }

    if (num == 0) {
        stop_playback();
    }
}


void pcm_tones_init() {
    // Setup PORTD PIN 6 as an ouput (OC0A)
    DDRD |= _BV(PD6);

    // Set up Timer 0 to do pulse width modulation on the speaker
    // pin.

    // Set fast PWM mode  (p.157)
    TCCR0A |= _BV(WGM01) | _BV(WGM00);
    TCCR0B &= ~_BV(WGM02);

    // Do non-inverting PWM on pin OC0A (p.155)
    TCCR0A |= _BV(COM0A1);
    TCCR0A &= ~_BV(COM0A0);

    // Set initial pulse width to the first sample.
    OCR0A = PCM_LOW;

    // No prescaler (p.158) (enables timer)
    TCCR0B &= ~_BV(CS02) & ~_BV(CS01);
    // TCCR0B |= _BV(CS00);
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
    if (current_mode == PLAYING) {
        return -1;
    } else {
        return 0;
    }
};
