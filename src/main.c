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

    printf("dow: ");
    retval = get_bcd(&date.dow, 2);
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
    date.dow = bcd_to_dec(date.dow);

    ds3231_set(&date);
}


int main(void)
{
    datetime_t last_date = {0};
    datetime_t date;
    valarm_t alarms[NUM_ALARMS];
    uint8_t alarm_already_active;
    const unsigned char * lut;

    /* init */
    uart_init(38400);
    stdout = &uart_stdout;
    stdin = &uart_input;
    printf("init\n");
    ds3231_init();
    pcm_audio_init();
    lut = lut_full_update;
    epd_Init(lut);
    epd_Sleep();

    view_init();

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
    alarms[2].hour = 11;
    alarms[2].minute = 31;
    alarms[2].dow = 0xff;
    // alarms[2].active = 1;

#ifdef RESET_DATE
    set_date();
#endif

    while (1) {
        ds3231_get(&date);
        if (
            date.year != last_date.year || \
            date.month != last_date.month || \
            date.day != last_date.day || \
            date.hours != last_date.hours || \
            date.minutes != last_date.minutes
        ) {
            if (
                date.year != last_date.year || \
                date.month != last_date.month || \
                date.day != last_date.day || \
                date.hours != last_date.hours
            ) {
                lut = lut_full_update;
            }
            alarm_already_active = activated_alarms(alarms);
            check_alarms(alarms, &date);
            epd_Init(lut);
            epd_ClearFrameMemory(0xff);
            view_update(&date, alarms);
            epd_DisplayFrame();
            epd_Sleep();
            lut = lut_partial_update;
            if (activated_alarms(alarms) && !alarm_already_active) {
                printf("Activating alarm!");
                start_alarm(alarms);
            }
            memcpy(&last_date, &date, sizeof(date));
        }

        printf(
            "%02u:%02u:%02u %04u-%02u-%02u %02u %s\n",
            date.hours,
            date.minutes,
            date.seconds,
            date.year,
            date.month,
            date.day,
            date.dow,
            datetime_DOW[date.dow]
        );

        _delay_ms(1000);
    }

    return 0;
}
