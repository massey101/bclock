// This code is designed to support the bclock or Barrukka Clock.

#include <util/delay.h>
#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>
#include <string.h>

#include "uart.h"
#include "epd2in13b.h"
#include "epdpaint.h"
#include "ds3231.h"
#include "clock.h"
#include "selected_clock.h"


#define COLORED   0
#define UNCOLORED 1

uint8_t BCDtoDEC(uint8_t bcd_val) {
    return (((bcd_val >> 4) * 10) + (bcd_val & 0x0F));
}


uint8_t get_bcd_char(uint8_t * result) {
    *result = getchar();
    if (*result < 0x30 || *result > 0x39) {
        return 0xff;
    }

    printf("%c", *result);
    *result -= 0x30;

    return 0x00;
}

uint8_t get_bcd(uint8_t * result, uint8_t length) {
    if (length % 2 != 0) {
        return 0xff;
    }

    for (uint8_t i = 0; i < length; i++) {
        uint8_t input;
        if(get_bcd_char(&input)) {
            return 0xff;
        }

        if (i % 2 == 0) {
            result[i/2] = input << 4;
        } else {
            result[i/2] |= input;
        }
    }

    return 0x00;
}


void set_date() {
    datetime_t date;
    uint8_t retval;

    printf("hour: ");
    retval = get_bcd(&date.hours, 2);
    printf("\n");
    if (retval) {
        return;
    }

    printf("minute: ");
    retval = get_bcd(&date.minutes, 2);
    printf("\n");
    if (retval) {
        return;
    }

    printf("second: ");
    retval = get_bcd(&date.seconds, 2);
    printf("\n");
    if (retval) {
        return;
    }

    uint8_t year_small;

    printf("year: ");
    retval = get_bcd(&year_small, 2);
    printf("\n");
    if (retval) {
        return;
    }

    printf("month: ");
    retval = get_bcd(&date.month, 2);
    printf("\n");
    if (retval) {
        return;
    }

    printf("date: ");
    retval = get_bcd(&date.day, 2);
    printf("\n");
    if (retval) {
        return;
    }

    date.hours = bcd_to_dec(date.hours);
    date.minutes = bcd_to_dec(date.minutes);
    date.seconds = bcd_to_dec(date.seconds);
    date.year = bcd_to_dec(year_small) + 2000;
    date.month= bcd_to_dec(date.month);
    date.day = bcd_to_dec(date.day);

    ds3231_set(&date);
}

void show_time(struct epd * epd, struct paint * paint, datetime_t * datetime) {
    char time_text[16];
    char date_text[16];
    sprintf(
        time_text,
        "%02u:%02u",
        datetime->hours,
        datetime->minutes
    );
    sprintf(
        date_text,
        "%02u/%02u/%04u",
        datetime->day,
        datetime->month,
        datetime->year
    );
    paint_SetWidth(paint, 40);
    paint_SetHeight(paint, 130);
    paint_Clear(paint, UNCOLORED);
    paint_DrawStringAt(paint, 0, 0, time_text, &Courier_New24, 2, COLORED);
    epd_set_partial_window_black(
        epd,
        paint_GetImage(paint),
        epd->width - paint->width-2,
        8,
        paint_GetWidth(paint),
        paint_GetHeight(paint)
    );
    paint_Clear(paint, UNCOLORED);
    paint_DrawStringAt(paint, 0, 0, date_text, &Courier_New24, 1, COLORED);
    epd_set_partial_window_black(
        epd,
        paint_GetImage(paint),
        epd->width - paint->width-50,
        8,
        paint_GetWidth(paint),
        paint_GetHeight(paint)
    );

    epd_display_frame(epd);
}


int main(void)
{
    datetime_t last_date = {0};
    datetime_t date;
    unsigned char canvas[1024];
    struct epd epd;
    struct paint paint;

    /* init */
    uart_init(38400);
    ds3231_init();
    epd_init(&epd);
    paint_init(&paint, canvas, 0, 0);
    stdout = &uart_stdout;
    stdin = &uart_input;

    paint_SetRotate(&paint, ROTATE_90);

    // Reset with all white
    epd_clear_frame_memory(&epd);
#ifdef RESET_DATE
    set_date();
#endif
    paint_SetWidth(&paint, 16);
    paint_SetHeight(&paint, 16);
    paint_Clear(&paint, UNCOLORED);
    paint_DrawImageAt(&paint, 0, 0, clock, 1, COLORED);
    epd_set_partial_window_black(
        &epd,
        paint_GetImage(&paint),
        epd.width - paint.width-8,
        150,
        paint_GetWidth(&paint),
        paint_GetHeight(&paint)
    );

    paint_Clear(&paint, UNCOLORED);
    paint_DrawImageAt(&paint, 0, 0, selected_clock, 1, COLORED);
    epd_set_partial_window_black(
        &epd,
        paint_GetImage(&paint),
        epd.width - paint.width-32,
        150,
        paint_GetWidth(&paint),
        paint_GetHeight(&paint)
    );

    while(1) {
        ds3231_get(&date);
        if (
            date.year != last_date.year || \
            date.month != last_date.month || \
            date.day != last_date.day || \
            date.hours != last_date.hours || \
            date.minutes != last_date.minutes
        ) {
            show_time(&epd, &paint, &date);
            memcpy(&last_date, &date, sizeof(date));
        }

        printf(
            "%02u:%02u:%02u %04u-%02u-%02u\n",
            date.hours,
            date.minutes,
            date.seconds,
            date.year,
            date.month,
            date.day
        );

        _delay_ms(1000);
    }

    return 0;
}
