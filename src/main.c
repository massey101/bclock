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
#include "pcm_tones.h"
#include "pam8403.h"
#include "reactor.h"
#include "buttons.h"
#include "ui.h"


#define BUTTON_QUEUE_SIZE 16


// Global State variables
vdatetime_t last_date = {0};
vdatetime_t date;
valarm_t alarms[NUM_ALARMS];
static const unsigned char * volatile lut;
volatile ms_t ms_since_last_draw = INT32_MAX;
volatile ms_t ms_since_last_minute = INT32_MAX;
char button_queue[BUTTON_QUEUE_SIZE] = {0};
uint8_t button_queue_start = 0;
uint8_t button_queue_end = 0;
mHz_t tone_freq = 440000;



void stop_alarm_cb() {
    clear_alarms(alarms);
    pcm_tones_stop();
    pam8403_disable();
}

void start_alarm_cb(mHz_t freq) {
    alarms[0].active = -1;
    pam8403_enable();
    reactor_call_later(TASK_AUDIO, 10);
    tone_freq = freq;
}

void button_pressed_cb(char input) {
    // Check that we haven't run into the end of our queue.
    // Note that this ignores the wrap around edge case.
    if (button_queue_end == button_queue_start - 1) {
        return;
    }

    // Check the edge case of the above condition.
    if (button_queue_end == BUTTON_QUEUE_SIZE - 1) {
        if (button_queue_start == 0) {
            return;
        }
    }

    // Store the input in the queue and advance it if necassary.
    button_queue[button_queue_end] = input;
    button_queue_end += 1;
    if (button_queue_end >= BUTTON_QUEUE_SIZE) {
        button_queue_end = 0;
    }

    reactor_call_later(TASK_UI, 5);
    reactor_update();
}

void tone_done_cb();


void force_redraw_now_cb(uint8_t full_update) {
    if (full_update) {
        lut = lut_full_update;
    }

    reactor_call_later(TASK_DISPLAY, 3);
}


void ui_task(ms_t real_ms) {
    // Process all the buttons in the queue ensuring we wrap around.
    int ui_force_redraw = 0;
    while (button_queue_start != button_queue_end) {
        int force_redraw = ui_input(button_queue[button_queue_start]);
        if (force_redraw > ui_force_redraw) {
            ui_force_redraw = force_redraw;
        }
        button_queue_start += 1;
        if (button_queue_start >= BUTTON_QUEUE_SIZE) {
            button_queue_start = 0;
        }
    }

    force_redraw_now_cb(ui_force_redraw == 2);
}


void setup() {
    /* init */
    uart_init(38400);
    stdout = &uart_stdout;
    stdin = &uart_input;
    printf("init\n");
    buttons_init(&button_pressed_cb);
    ds3231_init();
    pcm_tones_init();
    pam8403_init();
    lut = lut_full_update;
    epd_Init(lut);
    epd_Sleep();

    view_init();
    ui_init(&date, &last_date, alarms, &force_redraw_now_cb, &stop_alarm_cb, &start_alarm_cb);

    init_alarms(alarms);
}


void print_date(const vdatetime_t * date) {
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
    printf("DONE\n");
    pcm_tones_stop();
    reactor_enable_sleep();

    if (activated_alarms(alarms)) {
        reactor_call_later(TASK_AUDIO, 1000);
    } else {
        pam8403_disable();
    }
}

void audio_done_task(ms_t real_ms) {
    if (pcm_tones_busy()) {
        reactor_call_later(TASK_AUDIO_DONE, 10);
        return;
    }

    printf("Done Playing\n");

    reactor_enable_sleep();

    if (activated_alarms(alarms)) {
        reactor_call_later(TASK_AUDIO, 3000);
    } else {
        pam8403_disable();
    }
}

void audio_task(ms_t real_ms) {
    if (! activated_alarms(alarms)) {
        pam8403_disable();
        return;
    }

    if (pcm_tones_busy()) {
        reactor_call_later(TASK_AUDIO, 100);
    }

    reactor_disable_sleep();
    printf("PLAYING\n");
    // pcm_tones_play(tone_freq, 10000, 7, &tone_done_cb);
    // pcm_tones_play(100000, 10000, 7, &tone_done_cb);
    // pcm_tones_play(550000, 5000, 7, &tone_done_cb);
    // pcm_tones_play(261630, 5000, 7, &tone_done_cb);
    pcm_tones_play(349230, 5000, 7, &tone_done_cb);
    pcm_tones_play(440000, 5000, 6, &tone_done_cb);
    pcm_tones_play(523250, 5000, 7, &tone_done_cb);
    pcm_tones_play(659260, 5000, 7, &tone_done_cb);
    reactor_call_later(TASK_AUDIO_DONE, 10);
}


void display_sleep_task(ms_t real_ms) {
    ms_since_last_draw += real_ms;

    if (epd_IsAsleep()) {
        return;
    }

    if (epd_IsBusy()) {
        reactor_call_later(TASK_DISPLAY_SLEEP, 10);
        return;
    }

    if (ms_since_last_draw < 200) {
        reactor_call_later(TASK_DISPLAY_SLEEP, 10);
        return;
    }

    printf("SLEEP\n");
    epd_Sleep();
}


void display_task(ms_t real_ms) {
    // Update the display and then immediately put it back to sleep.
    // That part is important.
    if (epd_IsBusy()) {
        reactor_call_later(TASK_DISPLAY, 10);
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

    reactor_call_later(TASK_DISPLAY_SLEEP, 10);
}


void do_alarm_check() {
    uint8_t alarm_already_active = activated_alarms(alarms);
    check_alarms(alarms, &date);
    if (activated_alarms(alarms) && !alarm_already_active) {
        printf("Activating alarm!\n");
        pam8403_enable();
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

            if (! datetime_cmp(&date, &last_date, DAYS)) {
                lut = lut_full_update;
            }

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


// int free_ram() {
//     extern int __heap_start, *__brkval;
//     int v;
//     return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
// }

void led_task(ms_t realms) {
    uint8_t c_val = PINB & _BV(PB0);
    if (c_val) {
        PORTB &= ~_BV(PB0);
        reactor_call_later(TASK_LED, 900);
    } else {
        PORTB |= _BV(PB0);
        reactor_call_later(TASK_LED, 100);
        // printf("ram: %d/2048\n", free_ram());
    }
}

int main(void)
{
    setup();

    DDRB |= _BV(PB0);

    reactor_call_later(TASK_TIMER, 0);
    reactor_call_later(TASK_LED, 0);

    _delay_ms(10);
    reactor();

    return 0;
}
