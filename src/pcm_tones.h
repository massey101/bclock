/*
 * Plays 8-bit PCM audio on pin 11 using pulse-width modulation (PWM).
 * For Arduino with Atmega328 at 8 MHz.
 *
 * Uses two timers. The first changes the sample value 10000 times a second.
 * The second outputs an 255 step "analogue" value using PWM for a speaker.
 * This analogue value is produced using a low pass filter and an amp.
 *
 * References:
 *     Based on this example by Michael Smith:
 *     https://playground.arduino.cc/Code/PCMAudio
 */

#ifndef PCM_TONES_H
#define PCM_TONES_H

#include "sleep.h"
#include <stdint.h>
#include <avr/pgmspace.h>

typedef void (* volatile pcm_tones_cb_t)();

typedef uint32_t mHz_t;

void pcm_tones_init();
void pcm_tones_play(
    mHz_t frequency,
    ms_t duration,
    uint8_t volume,
    pcm_tones_cb_t _done_cb
);
void pcm_tones_stop();
uint8_t pcm_tones_busy();

#endif // PCM_TONES_H
