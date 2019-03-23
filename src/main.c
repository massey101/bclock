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
#include "buttons.h"
#include "sounds.h"
#include "ui.h"


// Global State variables
vdatetime_t last_date = {0};
vdatetime_t date;
valarm_t alarms[NUM_ALARMS];
const unsigned char * volatile lut;
volatile ms_t ms_since_last_draw = INT32_MAX;
volatile ms_t ms_since_last_minute = INT32_MAX;



void stop_alarm_cb() {
    clear_alarms(alarms);
    pcm_audio_stop();
}


void button_pressed_cb(char input) {
    ui_input(input);
}


void force_redraw_now_cb(uint8_t full_update) {
    if (full_update) {
        lut = lut_full_update;
    }

    reactor_call_later(TASK_DISPLAY, 100);
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


void tone_done_cb() {
    pcm_audio_stop();
    reactor_enable_sleep();

    if (activated_alarms(alarms)) {
        reactor_call_later(TASK_AUDIO, 5000);
    }
}


void audio_task(ms_t real_ms) {
    if (! activated_alarms(alarms)) {
        return;
    }

    if (pcm_audio_busy()) {
        reactor_call_later(TASK_AUDIO, 100);
    }

    reactor_disable_sleep();
    pcm_audio_play(&snd_buzzer, &tone_done_cb);
}


void display_sleep_task(ms_t real_ms) {
    ms_since_last_draw += real_ms;

    if (epd_IsAsleep()) {
        return;
    }

    if (epd_IsBusy()) {
        reactor_call_later(TASK_DISPLAY_SLEEP, 100);
        return;
    }

    if (ms_since_last_draw < 4000) {
        reactor_call_later(TASK_DISPLAY_SLEEP, 100);
        return;
    }

    printf("SLEEP\n");
    epd_Sleep();
}


void display_task(ms_t real_ms) {
    // Update the display and then immediately put it back to sleep.
    // That part is important.
    if (epd_IsBusy()) {
        reactor_call_later(TASK_DISPLAY, 100);
        return;
    }

    if (epd_IsAsleep()) {
        printf("WAKE\n");
        epd_Init(lut);
    }
    epd_ClearFrameMemory(0xff);
    view_update(&date, alarms);
    epd_DisplayFrame();

    // Reset display updating variables;
    ms_since_last_draw = 0;
    lut = lut_partial_update;

    reactor_call_later(TASK_DISPLAY_SLEEP, 100);
}


void do_alarm_check() {
    uint8_t alarm_already_active = activated_alarms(alarms);
    check_alarms(alarms, &date);
    if (activated_alarms(alarms) && !alarm_already_active) {
        printf("Activating alarm!\n");
        reactor_call_later(TASK_AUDIO, 10);
    };
}


void timer_task(ms_t real_ms) {
    ms_since_last_minute += real_ms;

    // Until we get to around 50 seconds don't bother checking the time and
    // don't bother sleeping for a short time.
    // Once we get past 50 seconds start checking the time every minute.
    //
    // If we predict that the seconds counter is approximately higher than 50s
    // then start checking the time every second.
    ms_t sleep_for_ms = 1000;
    if (ms_since_last_minute > 50000) {
        ds3231_get(&date);
        ms_since_last_minute = 1000 * (int32_t) date.second;
        print_date(&date);

        if (! datetime_cmp(&date, &last_date, MINUTES)) {
            do_alarm_check();

            reactor_call_later(TASK_DISPLAY, 5);
        }

        datetime_copy(&last_date, &date);

    }

    // If we predict that the seconds counter is approximately lower than 50s
    // then don't bother checking the time, just wait until we get there.
    if (ms_since_last_minute < 50000) {
        sleep_for_ms = 50000 - ms_since_last_minute;
    }

    reactor_call_later(TASK_TIMER, sleep_for_ms);
}


void led_task(ms_t realms) {
    uint8_t c_val = PINC & _BV(PC3);
    if (c_val) {
        PORTC &= ~_BV(PC3);
        reactor_call_later(TASK_LED, 900);
    } else {
        PORTC |= _BV(PC3);
        reactor_call_later(TASK_LED, 100);
    }
}



int main(void)
{
    setup();

    DDRC |= _BV(PC3);

    reactor_call_later(TASK_TIMER, 0);
    reactor_call_later(TASK_LED, 0);

    _delay_ms(1000);
    reactor();

    return 0;
}
