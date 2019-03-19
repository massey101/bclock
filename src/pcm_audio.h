/*
 * Plays 8-bit PCM audio on pin 11 using pulse-width modulation (PWM).
 * For Arduino with Atmega168 at 16 MHz.
 *
 * Uses two timers. The first changes the sample value 8000 times a second.
 * The second outputs an 255 step "analogue" value using PWM for a speaker.
 * This could be greatly improved using a low pass filter and an amp.
 *
 * References:
 *     Based on this example by Michael Smith:
 *     https://playground.arduino.cc/Code/PCMAudio
 */

#ifndef PCM_AUDIO_H
#define PCM_AUDIO_H

#include <stdint.h>
#include <avr/pgmspace.h>
#include "callbacks.h"

struct pcm_audio {
    const uint16_t sample_rate;
    const uint16_t length;
    const uint8_t * data;
};

void pcm_audio_init();
void pcm_audio_play(
    const struct pcm_audio * pcm_audio,
    cb_t _done_cb,
    pctx_t _done_cb_ctx
);
void pcm_audio_stop();
uint8_t pcm_audio_busy();

#endif // PCM_AUDIO_H
