// This code is designed to support the bclock or Barrukka Clock.

#include <util/delay.h>
#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>

#include "uart.h"
#include "spi.h"
#include "st7735.h"
#include "st7735_gfx.h"
#include "st7735_font.h"
#include "ds3231.h"

#include "fonts/tom_thumb.h"


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

void show_time(datetime_t * datetime) {
    char time_text[16];
    char date_text[16];
    sprintf(
        time_text,
        "%02u:%02u:%02u",
        datetime->hours,
        datetime->minutes,
        datetime->seconds
    );
    sprintf(
        date_text,
        "%02u/%02u/%04u",
        datetime->day,
        datetime->month,
        datetime->year
    );
    //st7735_fill_rect(0, 0, 128, 128, ST7735_COLOR_WHITE);
    st7735_draw_text(
        5,
        25,
        time_text,
        &TomThumb,
        4,
        ST7735_COLOR_BLACK,
        ST7735_COLOR_WHITE
    );
    st7735_draw_text(
        5,
        50,
        date_text,
        &TomThumb,
        3,
        ST7735_COLOR_BLACK,
        ST7735_COLOR_WHITE
    );
}


int main(void)
{
    datetime_t date;

    /* init */
    spi_init();
    ds3231_init();
    st7735_init();
    st7735_set_orientation(ST7735_LANDSCAPE);
    uart_init(38400);
    stdout = &uart_stdout;
    stdin = &uart_input;

    // Reset with all white
    st7735_fill_rect(0, 0, 128, 128, ST7735_COLOR_WHITE);
    set_date();


    while(1) {
        ds3231_get(&date);
        printf(
            "%02u:%02u:%02u %04u-%02u-%02u\n",
            date.hours,
            date.minutes,
            date.seconds,
            date.year,
            date.month,
            date.day
        );
        show_time(&date);

        _delay_ms(1000);
    }

    return 0;
}
