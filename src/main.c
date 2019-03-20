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


vuart_ctx_t uart_ctx;
vdatetime_t last_date = {0};
vdatetime_t date;
valarm_t alarms[NUM_ALARMS];
const unsigned char * volatile lut;
volatile uint8_t force_redraw;


void button_pressed(pctx_t pctx) {
    uart_ctx_t * uart_ctx = (uart_ctx_t *)pctx;

    printf("Got: %d\n", uart_ctx->data);

    ui_input(uart_ctx->data);
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
    uart_ctx.data = 0;
    uart_ctx.ctx = 0;
    uart_init_interrupt(38400, &button_pressed, &uart_ctx);
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


void draw_loop(pctx_t pctx) {

    ds3231_get(&date);

    // Update the display. This is only triggered if there is a change in
    // time or if it is forced to by the ui.
    if (! datetime_cmp(&date, &last_date, MINUTES) || force_redraw) {
        printf("Updating display\n");

        // Perform a full update of the screen every hour or when forced to by
        // the ui.
        if (! datetime_cmp(&date, &last_date, HOURS) || force_redraw == 2) {
            printf("Full update\n");
            lut = lut_full_update;
        }

        // Update the display and then immediately put it back to sleep.
        // That part is important.
        epd_Init(lut);
        epd_ClearFrameMemory(0xff);
        view_update(&date, alarms);
        epd_DisplayFrame();
        epd_Sleep();
        lut = lut_partial_update;
        force_redraw = 0;
    }

    // Check if any alarms need activating.
    uint8_t alarm_already_active = activated_alarms(alarms);
    check_alarms(alarms, &date);
    if (activated_alarms(alarms) && !alarm_already_active) {
        printf("Activating alarm!\n");
        start_alarm(alarms);
    };

    printf("date1: ");
    print_date(&date);
    printf("date2: ");
    print_date(&last_date);
    printf("\n");

    datetime_copy(&last_date, &date);

    async_delay_ms(1000, &draw_loop, 0);
}


int main(void)
{
    setup();

    draw_loop(0);

    while (1);

    return 0;
}
