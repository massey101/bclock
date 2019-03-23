// This code is designed to support the bclock or Barrukka Clock.

#include <util/delay.h>
#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>
#include <string.h>

#include "uart.h"
#include "epd2in13.h"
#include "epdpaint.h"
#include "view.h"
#include "ds3231.h"
#include "alarms.h"
#include "pcm_audio.h"
#include "reactor.h"
#include "tasks.h"
#include "buttons.h"
#include "sounds.h"
#include "ui.h"


vdatetime_t last_date = {0};
vdatetime_t date;
valarm_t alarms[NUM_ALARMS];
const unsigned char * volatile lut;
volatile uint8_t force_redraw = 0;
volatile ms_t ms_since_last_draw = INT32_MAX;
volatile ms_t ms_since_last_minute = INT32_MAX;
volatile ms_t ms_since_last_time_fetch = INT32_MAX;
volatile ms_t ms_since_tone_finish = INT32_MAX;


void stop_alarm_cb() {
    clear_alarms(alarms);
    pcm_audio_stop();
}


void button_pressed_cb(char input) {
    ui_input(input);
}


void force_redraw_now_cb(uint8_t full_update) {
    force_redraw = 1;
    if (full_update) {
        force_redraw = 2;
    }

    reactor_trigger(TASK_MAIN, 100);
    reactor_update();
}


void setup() {
    /* init */
    uart_init_interrupt(38400, &button_pressed_cb);
    stdout = &uart_stdout;
    stdin = &uart_input;
    printf("init\n");
    buttons_init(&button_pressed_cb);
    ds3231_init();
    pcm_audio_init();
    lut = lut_full_update;
    epd_Init(lut);
    epd_Sleep();

    view_init();
    ui_init(&date, &last_date, alarms, &force_redraw_now_cb, &stop_alarm_cb);

    init_alarms(alarms);
    alarms[0].set = 1;
    alarms[0].hour = 7;
    alarms[0].minute = 30;
    alarms[0].dow = 0x3e;

    alarms[1].set = 1;
    alarms[1].hour = 8;
    alarms[1].minute = 30;
    alarms[1].dow = 0xc0;

    alarms[2].set = 1;
    alarms[2].hour = 12;
    alarms[2].minute = 55;
    alarms[2].dow = 0xff;
}


void print_date(vdatetime_t * date) {
   printf(
        "%02u:%02u:%02u %04u-%02u-%02u %02u %s\n",
        date->hour,
        date->minute,
        date->second,
        date->year,
        date->month,
        date->day,
        date->dow,
        datetime_DOW[date->dow]
    );
};


typedef ms_t (*watcher_t)(void);


ms_t watch(watcher_t watcher, ms_t sleep_for_ms) {
    ms_t watcher_wants_ms = watcher();
    if (watcher_wants_ms && watcher_wants_ms < sleep_for_ms) {
        sleep_for_ms = watcher_wants_ms;
    }

    return sleep_for_ms;
}


ms_t display_update_watcher() {
    // Update the display and then immediately put it back to sleep.
    // That part is important.
    if (epd_IsBusy()) {
        return 100;
    }

    force_redraw = 0;
    ms_since_last_draw = 0;

    if (epd_IsAsleep()) {
        printf("WAKE\n");
        epd_Init(lut);
    }
    epd_ClearFrameMemory(0xff);
    view_update(&date, alarms);
    epd_DisplayFrame();

    // Reset display updating variables;
    lut = lut_partial_update;

    return 0;
}


ms_t display_sleep_watcher() {
    if (epd_IsAsleep()) {
        return 0;
    }

    if (epd_IsBusy()) {
        return 100;
    }

    if (ms_since_last_draw < 2000) {
        return 100;
    }

    printf("SLEEP\n");
    epd_Sleep();
    return 0;
}


void tone_done_cb() {
    ms_since_tone_finish = 0;
    pcm_audio_stop();
    reactor_enable_sleep();
}


ms_t audio_watcher() {
    if (! activated_alarms(alarms)) {
        return 0;
    }

    if (pcm_audio_busy()) {
        return 1000;
    }

    if (ms_since_tone_finish > 5000) {
        reactor_disable_sleep();
        pcm_audio_play(&snd_buzzer, &tone_done_cb);
        return 1000;
    }

    return 1000;
}


void main_task(ms_t real_ms) {
    ms_since_last_draw += real_ms;
    ms_since_last_minute += real_ms;
    ms_since_last_time_fetch += real_ms;
    if (activated_alarms(alarms)) {
        ms_since_tone_finish += real_ms;
    }

    // Until we get to around 50 seconds don't bother checking the time and
    // don't bother sleeping for a short time.
    // Once we get past 50 seconds start checking the time every minute.
    //
    // If we predict that the seconds counter is approximately higher than 50s
    // then start checking the time every second.
    ms_t sleep_for_ms = 1000;
    if (ms_since_last_minute > 50000) {
        if (ms_since_last_time_fetch >= 1000) {
            ds3231_get(&date);
            ms_since_last_time_fetch = 0;
            ms_since_last_minute = 1000 * (int32_t) date.second;
            print_date(&date);
        }
    }

    // If we predict that the seconds counter is approximately lower than 50s
    // then don't bother checking the time, just wait until we get there.
    if (ms_since_last_minute < 50000) {
        sleep_for_ms = 50000 - ms_since_last_minute;
    }


    if (! datetime_cmp(&date, &last_date, MINUTES)) {
        // Check if any alarms need activating.
        uint8_t alarm_already_active = activated_alarms(alarms);
        check_alarms(alarms, &date);
        if (activated_alarms(alarms) && !alarm_already_active) {
            printf("Activating alarm!\n");
            ms_since_tone_finish = 0xffffffff;
        };
    }

    // Update the display. This is only triggered if there is a change in
    // time or if it is forced to by the ui.
    if (! datetime_cmp(&date, &last_date, MINUTES) || force_redraw) {

        // Perform a full update of the screen every hour or when forced to by
        // the ui.
        if (! datetime_cmp(&date, &last_date, HOURS) || force_redraw == 2) {
            lut = lut_full_update;
        }

        sleep_for_ms = watch(display_update_watcher, sleep_for_ms);


    }

    // Check whether we should start playing a sound
    sleep_for_ms = watch(audio_watcher, sleep_for_ms);

    // Check whether the display watcher wants to put the display in sleep mode
    // (IMPORTANT)
    sleep_for_ms = watch(display_sleep_watcher, sleep_for_ms);

    datetime_copy(&last_date, &date);


    // Double check whether we have had a second force redraw issued since the
    // display update. If so we want to make sure we update the display again
    // soonish.
    if (force_redraw && sleep_for_ms > 500) {
        sleep_for_ms = 500;
    }

    reactor_call_later(TASK_MAIN, &main_task, sleep_for_ms);
}


void flash_led_task(ms_t realms) {
    uint8_t c_val = PINC & _BV(PC3);
    if (c_val) {
        PORTC &= ~_BV(PC3);
        reactor_call_later(5, &flash_led_task, 900);
    } else {
        PORTC |= _BV(PC3);
        reactor_call_later(5, &flash_led_task, 100);
    }
}



int main(void)
{
    setup();

    DDRC |= _BV(PC3);

    reactor_call_later(TASK_MAIN, &main_task, 0);
    reactor_call_later(5, &flash_led_task, 0);

    _delay_ms(1000);
    reactor();

    return 0;
}
