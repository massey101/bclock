#include "view.h"
#include <stdio.h>
#include "epd2in13.h"
#include "img_bell.h"
#include "img_bell_ringing.h"
#include "img_arrow.h"
#include "img_arrow_down.h"
#include "img_arrow_up.h"
#include "ui.h"


static unsigned char canvas[1024];
static struct paint paint;


void show_time(int x, int y, vdatetime_t * datetime) {
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
        2000+datetime->year
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


void show_alarm_index(int x, int y, uint8_t i) {
    char num[2] = {'1' + i, '\0'};
    paint_DrawStringAt(&paint, x, y, num, &Courier_New12, 1, COLORED);
}


void show_alarm_bell(int x, int y, valarm_t * alarm) {
    const unsigned char * to_draw;

    if (alarm->active) {
        to_draw = img_bell_ringing;
    } else {
        to_draw = img_bell;
    }

    paint_DrawImageAt(&paint, x, y, to_draw, 1, COLORED);
}


void show_alarm_time(int x, int y, valarm_t * alarm) {
    char time_text[16];
    sprintf(
        time_text,
        "%02u:%02u",
        alarm->hour,
        alarm->minute
    );

    paint_DrawStringAt(&paint, x, y, time_text, &Courier_New12, 1, COLORED);
}


void show_alarm_dow(int x, int y, valarm_t * alarm) {
    const char DOW[] = "MTWTFSS";
    char alarm_DOW[] = "       ";
    for (int i = 0; i < 7; i++) {
        if (alarm->dow & (0x01 << (i + 1))) {
            alarm_DOW[i] = DOW[i];
        }
    }
    paint_DrawStringAt(&paint, x, y, alarm_DOW, &Courier_New12, 1, COLORED);
}


void show_alarm_pointer() {
    int x, y, up;
    switch(current_state) {
        case SET_ALARM_HOUR1:         x = 18+7*1; y = 0; up = 0; break;
        case SET_ALARM_HOUR2:         x = 18+7*2; y = 0; up = 0; break;
        case SET_ALARM_MINUTE1:       x = 18+7*4; y = 0; up = 0; break;
        case SET_ALARM_MINUTE2:       x = 18+7*5; y = 0; up = 0; break;
        case SET_ALARM_DOW_MONDAY:    x = 18+7*1; y = 28; up = 1; break;
        case SET_ALARM_DOW_TUESDAY:   x = 18+7*2; y = 28; up = 1; break;
        case SET_ALARM_DOW_WEDNESDAY: x = 18+7*3; y = 28; up = 1; break;
        case SET_ALARM_DOW_THURSDAY:  x = 18+7*4; y = 28; up = 1; break;
        case SET_ALARM_DOW_FRIDAY:    x = 18+7*5; y = 28; up = 1; break;
        case SET_ALARM_DOW_SATURDAY:  x = 18+7*6; y = 28; up = 1; break;
        case SET_ALARM_DOW_SUNDAY:    x = 18+7*7; y = 28; up = 1; break;
        default: x = 0; y = 0; up = -1; break;
    }
    if (up == -1) {
        return;
    }
    const unsigned char * img = img_arrow_down;
    if (up) {
        img = img_arrow_up;
    }

    paint_DrawImageAt(&paint, x, y, img, 1, COLORED);
}

void show_alarm(int x, int y, valarm_t * alarm, uint8_t i) {
    paint_SetWidth(&paint, 32);
    paint_SetHeight(&paint, 100);
    paint_Clear(&paint, UNCOLORED);

    if (current_state == SET_ALARMS) {
        show_alarm_index(0, 13, i);
        show_alarm_bell(8, 10, alarm);
    }
    if (alarm->set) {
        if (current_state != SET_ALARMS) {
            show_alarm_bell(8, 10, alarm);
        }
        show_alarm_time(26, 8, alarm);
        show_alarm_dow(26, 16, alarm);
    }

    if (
        current_state >= SET_ALARM_HOUR1 &&
        current_state <= SET_ALARM_DOW_SUNDAY &&
        selected_alarm == i
    ) {
        show_alarm_pointer(26, 0, 26, 8);
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
        valarm_t * alarm = &alarms[i];
        if (
            current_state >= SET_ALARM_HOUR1 &&
            current_state <= SET_ALARM_DOW_SUNDAY &&
            selected_alarm == i
        ) {
            alarm = &new_alarm;
        }
        show_alarm(x, y + i*32, alarm, i);
    }
}


void view_init() {
    paint_init(&paint, canvas, 0, 0);
    paint_SetRotate(&paint, ROTATE_90);
}


void view_update(vdatetime_t * datetime, valarm_t * alarms) {
    show_time(8, 2, datetime);
    show_alarms(145, 8, alarms);
}
