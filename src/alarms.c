#include <avr/eeprom.h>
#include "alarms.h"
#include "sounds.h"
#include "async_delay.h"


// Define our callbacks and their parameters
typedef async_ctx struct {
    uint16_t counter;
    uint16_t delay;
    const struct pcm_audio * audio_to_play;
    valarm_t * alarms;
} alarm_ctx_t;

alarm_ctx_t ctx;
void wait_alarm_cb(pctx_t pctx);
void play_tone_alarm_cb(pctx_t pctx);


// Global variables stored in EEPROM
uint32_t EEMEM e_check_sum;
alarm_t EEMEM e_alarms[NUM_ALARMS];


// Checksum for EEPROM global variables
#define ALARM_CHECKSUM 0x2261cde8


void init_alarm(valarm_t * alarm) {
    alarm->set = 0;
    alarm->hour = 0;
    alarm->minute = 0;
    alarm->dow = 0;
}


void init_alarms(valarm_t * alarms) {
    uint32_t check_sum;

    check_sum = eeprom_read_dword(&e_check_sum);
    if (check_sum == ALARM_CHECKSUM) {
        for (int i = 0; i < NUM_ALARMS; i++) {
            eeprom_read_block((void *) &alarms[i], &e_alarms[i], sizeof(alarm_t));
        }
        return;
    }

    for (int i = 0; i < NUM_ALARMS; i++) {
        init_alarm(&alarms[i]);
        eeprom_write_block((void *) &alarms[i], &e_alarms[i], sizeof(alarm_t));
    }
    eeprom_write_dword(&e_check_sum, ALARM_CHECKSUM);
}


uint8_t check_alarm(valarm_t * alarm, datetime_t * date) {
    if (!alarm->set) {
       return 0;
    }

    if (alarm->hour != date->hours) {
        return 0;
    }

    if (alarm->minute != date->minutes) {
        return 0;
    }

    if (!(alarm->dow & (1<<date->dow))) {
        return 0;
    }

    alarm->active = -1;
    return -1;
}


/**
 * This callback will play a tone up to 20 times and will call wait_alarm_cb
 * when it has completed.
 */
void play_tone_alarm_cb(pctx_t pctx) {
    if (!activated_alarms(ctx.alarms)) {
        return;
    }

    if (ctx.counter >= 20) {
        clear_alarms(ctx.alarms);
        pcm_audio_stop();
        return;
    }

    ctx.counter++;
    pcm_audio_play(ctx.audio_to_play, &wait_alarm_cb, pctx);
}


/**
 * This callback will start an async delay to play the next tone.
 */
void wait_alarm_cb(pctx_t pctx) {
    pcm_audio_stop();
    if (!activated_alarms(ctx.alarms)) {
        return;
    }

    async_delay_ms(ctx.delay, &play_tone_alarm_cb, pctx);
}


void start_alarm(valarm_t * alarms) {
    ctx.counter = 0;
    ctx.delay = 5000;
    ctx.audio_to_play = &snd_buzzer;
    ctx.alarms = alarms;
    play_tone_alarm_cb(0);
}


uint8_t check_alarms(valarm_t * alarms, datetime_t * date) {
    for (int i = 0; i < NUM_ALARMS; i++) {
        if (check_alarm(&alarms[i], date)) {
            return -1;
        }
    }

    return 0;
}


uint8_t activated_alarms(valarm_t * alarms) {
    for (int i = 0; i < NUM_ALARMS; i++) {
        if (alarms[i].active) {
            return -1;
        }
    }

    return 0;
}

void clear_alarms(valarm_t * alarms) {
    for (int i = 0; i < NUM_ALARMS; i++) {
        alarms[i].active = 0;
    }
}
