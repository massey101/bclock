#include "view.h"
#include <stdio.h>
#include "epd2in13.h"
#include "img_bell.h"
#include "img_bell_ringing.h"


static unsigned char canvas[1024];
static struct paint paint;


void show_time(int x, int y, datetime_t * datetime) {
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
    paint_SetWidth(&paint, 40);
    paint_SetHeight(&paint, 130);
    paint_Clear(&paint, UNCOLORED);
    paint_DrawStringAt(&paint, 0, 0, time_text, &Courier_New24, 2, COLORED);
    epd_SetFrameMemory(
        paint_GetImage(&paint),
        epd_GetWidth() - paint_GetWidth(&paint) - y,
        x,
        paint_GetWidth(&paint),
        paint_GetHeight(&paint)
    );
    paint_SetWidth(&paint, 24);
    paint_Clear(&paint, UNCOLORED);
    paint_DrawStringAt(&paint, 0, 0, datetime_DOW[datetime->dow], &Courier_New24, 1, COLORED);
    epd_SetFrameMemory(
        paint_GetImage(&paint),
        epd_GetWidth() - paint_GetWidth(&paint) - y - 43,
        x,
        paint_GetWidth(&paint),
        paint_GetHeight(&paint)
    );
    paint_Clear(&paint, UNCOLORED);
    paint_DrawStringAt(&paint, 0, 0, date_text, &Courier_New24, 1, COLORED);
    epd_SetFrameMemory(
        paint_GetImage(&paint),
        epd_GetWidth() - paint_GetWidth(&paint) - y - 68,
        x,
        paint_GetWidth(&paint),
        paint_GetHeight(&paint)
    );
}


void show_alarm(int x, int y, valarm_t * alarm) {
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

    paint_SetWidth(&paint, 24);
    paint_SetHeight(&paint, 100);
    paint_Clear(&paint, UNCOLORED);
    paint_DrawImageAt(&paint, 0, 2, to_draw, 1, COLORED);

    if (alarm->set) {
        char time_text[16];
        sprintf(
            time_text,
            "%02u:%02u",
            alarm->hour,
            alarm->minute
        );

        paint_DrawStringAt(&paint, w+2, 0, time_text, &Courier_New12, 1, COLORED);
        const char DOW[] = "MTWTFSS";
        char alarm_DOW[] = "       ";
        for (int i = 0; i < 7; i++) {
            if (alarm->dow & (0x01 << (i + 1))) {
                alarm_DOW[i] = DOW[i];
            }
        }
        paint_DrawStringAt(&paint, w+2, 8, alarm_DOW, &Courier_New12, 1, COLORED);
    }

    epd_SetFrameMemory(
        paint_GetImage(&paint),
        epd_GetWidth() - paint_GetWidth(&paint) - y,
        x,
        paint_GetWidth(&paint),
        paint_GetHeight(&paint)
    );
}


void show_alarms(int x, int y, valarm_t * alarms) {
    for (int i = 0; i < NUM_ALARMS; i++) {
        show_alarm(x, y + i*24, &alarms[i]);
    }
}


void view_init() {
    paint_init(&paint, canvas, 0, 0);
    paint_SetRotate(&paint, ROTATE_90);
}


void view_update(datetime_t * datetime, valarm_t * alarms) {
    show_time(8, 2, datetime);
    show_alarms(145, 8, alarms);
}
