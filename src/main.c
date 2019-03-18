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
#include "alarms.h"
#include "pcm_audio.h"
#include "sounds.h"
#include "img_bell.h"
#include "img_bell_ringing.h"


#define COLORED   0
#define UNCOLORED 1


static volatile alarm_t alarms[NUM_ALARMS];
static volatile uint8_t audio_direction_toggle;


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
    paint_SetWidth(paint, 24);
    paint_Clear(paint, UNCOLORED);
    paint_DrawStringAt(paint, 0, 0, datetime_DOW[datetime->dow], &Courier_New24, 1, COLORED);
    epd_set_partial_window_black(
        epd,
        paint_GetImage(paint),
        epd->width - paint->width-45,
        8,
        paint_GetWidth(paint),
        paint_GetHeight(paint)
    );
    paint_Clear(paint, UNCOLORED);
    paint_DrawStringAt(paint, 0, 0, date_text, &Courier_New24, 1, COLORED);
    epd_set_partial_window_black(
        epd,
        paint_GetImage(paint),
        epd->width - paint->width-70,
        8,
        paint_GetWidth(paint),
        paint_GetHeight(paint)
    );
}

void draw_alarm(
    struct epd * epd,
    struct paint * paint,
    int x,
    int y,
    alarm_t * alarm
) {
    if (!alarm->set) {
        return;
    }
    const unsigned char * to_draw;
    if (alarm->active) {
        to_draw = img_bell_ringing;
    } else {
        to_draw = img_bell;
    }
    int w = pgm_read_byte(&to_draw[0]);

    paint_SetWidth(paint, 24);
    paint_SetHeight(paint, 100);
    paint_Clear(paint, UNCOLORED);
    paint_DrawImageAt(paint, 0, 2, to_draw, 1, COLORED);

    if (alarm->set) {
        char time_text[16];
        sprintf(
            time_text,
            "%02u:%02u",
            alarm->hour,
            alarm->minute
        );

        paint_DrawStringAt(paint, w+2, 0, time_text, &Courier_New12, 1, COLORED);
        const char DOW[] = "MTWTFSS";
        char alarm_DOW[] = "       ";
        for (int i = 0; i < 7; i++) {
            if (alarm->dow & (0x01 << (i + 1))) {
                alarm_DOW[i] = DOW[i];
            }
        }
        paint_DrawStringAt(paint, w+2, 8, alarm_DOW, &Courier_New12, 1, COLORED);
    }

    epd_set_partial_window_black(
        epd,
        paint_GetImage(paint),
        epd->width - paint_GetWidth(paint) - y,
        x,
        paint_GetWidth(paint),
        paint_GetHeight(paint)
    );
}


void draw_alarms(
    struct epd * epd,
    struct paint * paint,
    int x,
    int y,
    alarm_t * alarms
) {
    for (int i = 0; i < NUM_ALARMS; i++) {
        draw_alarm(epd, paint, x, y + i*24, &alarms[i]);
    }
}

void alarm_cb() {
    if (activated_alarms(alarms)) {
        pcm_audio_play(&snd_loop, audio_direction_toggle % 2, &alarm_cb);
        audio_direction_toggle++;
    }
}


void start_alarm() {
    audio_direction_toggle = 0;
    pcm_audio_play(&snd_start, 0, &alarm_cb);
}


int main(void)
{
    datetime_t last_date = {0};
    datetime_t date;
    unsigned char canvas[1024];
    struct epd epd;
    struct paint paint;
    uint8_t alarm_already_active;

    /* init */
    uart_init(38400);
    stdout = &uart_stdout;
    stdin = &uart_input;
    printf("init\n");
    ds3231_init();
    pcm_audio_init();
    epd_init(&epd);
    paint_init(&paint, canvas, 0, 0);

    paint_SetRotate(&paint, ROTATE_90);

    // Reset with all white
    epd_clear_frame_memory(&epd);

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
    alarms[2].hour = 0;
    alarms[2].minute = 59;
    alarms[2].dow = 0xff;

#ifdef RESET_DATE
    set_date();
#endif

    while(1) {
        ds3231_get(&date);
        if (
            date.year != last_date.year || \
            date.month != last_date.month || \
            date.day != last_date.day || \
            date.hours != last_date.hours || \
            date.minutes != last_date.minutes
        ) {
            alarm_already_active = activated_alarms(alarms);
            check_alarms(alarms, &date);
            epd_init(&epd);
            epd_clear_frame_memory(&epd);
            draw_alarms(&epd, &paint, 145, 8, alarms);
            show_time(&epd, &paint, &date);
            epd_display_frame(&epd);
            epd_sleep(&epd);
            if (activated_alarms(alarms) && !alarm_already_active) {
                printf("Activating alarm!");
                start_alarm();
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
