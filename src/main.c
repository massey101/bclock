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
#include "async_delay.h"
#include "sounds.h"
#include "ui.h"


vdatetime_t last_date = {0};
vdatetime_t date;
valarm_t alarms[NUM_ALARMS];
const unsigned char * volatile lut;
volatile uint8_t force_redraw = 0;
volatile uint32_t ms_since_last_draw = 0xffffffff;
volatile uint32_t ms_since_last_minute = 0xffffffff;
volatile uint32_t ms_since_last_time_fetch = 0xffffffff;


void button_pressed(char input) {
    ui_input(input);
}


void force_redraw_now(uint8_t full_update) {
    force_redraw = 1;
    if (full_update) {
        force_redraw = 2;
    }

    async_delay_trigger();
}


void setup() {
    /* init */
    uart_init_interrupt(38400, &button_pressed);
    stdout = &uart_stdout;
    stdin = &uart_input;
    printf("init\n");
    ds3231_init();
    pcm_audio_init();
    lut = lut_full_update;
    epd_Init(lut);
    epd_Sleep();

    view_init();
    ui_init(&date, &last_date, alarms, &force_redraw_now);

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
    alarms[2].minute = 01;
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


uint32_t update_display() {
    // Update the display and then immediately put it back to sleep.
    // That part is important.
    if (epd_IsBusy()) {
        return 100;
    }

    force_redraw = 0;
    ms_since_last_draw = 0;

    if (epd_IsAsleep()) {
        epd_Init(lut);
    }
    epd_ClearFrameMemory(0xff);
    view_update(&date, alarms);
    epd_DisplayFrame();

    // Reset display updating variables;
    lut = lut_partial_update;

    return 0;
}


uint32_t watch_display() {
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


void draw_loop(uint32_t real_ms) {
    ms_since_last_draw += real_ms;
    ms_since_last_minute += real_ms;
    ms_since_last_time_fetch += real_ms;

    // Until we get to around 50 seconds don't bother checking the time and
    // don't bother sleeping for a short time.
    // Once we get past 50 seconds start checking the time every minute.
    //
    // If we predict that the seconds counter is approximately higher than 50s
    // then start checking the time every second.
    uint32_t sleep_for_ms = 1000;
    if (ms_since_last_minute > 50000) {
        if (ms_since_last_time_fetch >= 1000) {
            ds3231_get(&date);
            ms_since_last_time_fetch = 0;
            ms_since_last_minute = 1000 * date.second;
            print_date(&date);
        }
    }

    // If we predict that the seconds counter is approximately lower than 50s
    // then don't bother checking the time, just wait until we get there.
    if (ms_since_last_minute < 50000) {
        sleep_for_ms = 50000 - ms_since_last_minute;
    }

    // Update the display. This is only triggered if there is a change in
    // time or if it is forced to by the ui.
    if (! datetime_cmp(&date, &last_date, MINUTES) || force_redraw) {

        // Perform a full update of the screen every hour or when forced to by
        // the ui.
        if (! datetime_cmp(&date, &last_date, HOURS) || force_redraw == 2) {
            lut = lut_full_update;
        }

        int display_updater_wants_ms = update_display();
        if (display_updater_wants_ms) {
            sleep_for_ms = display_updater_wants_ms;
        }
    }

    // Check if any alarms need activating.
    uint8_t alarm_already_active = activated_alarms(alarms);
    check_alarms(alarms, &date);
    if (activated_alarms(alarms) && !alarm_already_active) {
        printf("Activating alarm!\n");
        start_alarm(alarms);
    };

    datetime_copy(&last_date, &date);

    // Check whether the display watcher wants to put the display in sleep mode
    // (IMPORTANT)
    int display_watcher_wants_ms = watch_display();
    if (display_watcher_wants_ms) {
        sleep_for_ms = display_watcher_wants_ms;
    }

    // Double check whether we have had a second force redraw issued since the
    // display update. If so we want to make sure we update the display again
    // soonish.
    if (force_redraw && sleep_for_ms > 500) {
        sleep_for_ms = 500;
    }

    async_delay_ms(sleep_for_ms, &draw_loop);
}


int main(void)
{
    setup();

    draw_loop(0);

    while (1);

    return 0;
}
