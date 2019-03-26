#include "view.h"
#include <stdio.h>
#include "epd2in13.h"
#include "img_bell.h"
#include "img_bell_ringing.h"
#include "img_arrow.h"
#include "img_arrow_down.h"
#include "img_arrow_up.h"
#include "img_arrow_up_large.h"
#include "img_bell_disabled.h"
#include "ui.h"


static unsigned char canvas[1024];
static struct paint paint;


void show_time(int x, int y, vdatetime_t * datetime) {
    char time_text[16];
    sprintf(
        time_text,
        "%02u:%02u",
        datetime->hour,
        datetime->minute
    );
    paint_SetWidth(&paint, 40);
    paint_SetHeight(&paint, 144);
    paint_Clear(&paint, UNCOLORED);
    paint_DrawStringAt(&paint, 0, 10, time_text, &Courier_New48, 1, COLORED);
    epd_SetFrameMemory(
        paint_GetImage(&paint),
        epd_GetWidth() - paint_GetWidth(&paint) - y,
        x,
        paint_GetWidth(&paint),
        paint_GetHeight(&paint)
    );
}


void show_dow(int x, int y, vdatetime_t * datetime) {
    paint_SetWidth(&paint, 24);
    paint_SetHeight(&paint, 136);
    paint_Clear(&paint, UNCOLORED);
    paint_DrawStringAt(
        &paint,
        0,
        0,
        datetime_DOW[datetime->dow],
        &Courier_New24,
        1,
        COLORED
    );
    epd_SetFrameMemory(
        paint_GetImage(&paint),
        epd_GetWidth() - paint_GetWidth(&paint) - y,
        x,
        paint_GetWidth(&paint),
        paint_GetHeight(&paint)
    );
}


void show_date(int x, int y, vdatetime_t * datetime) {
    char date_text[16];
    paint_SetWidth(&paint, 24);
    paint_SetHeight(&paint, 136);
    paint_Clear(&paint, UNCOLORED);
    sprintf(
        date_text,
        "%02u/%02u/%04u",
        datetime->day,
        datetime->month,
        2000+datetime->year
    );
    paint_DrawStringAt(&paint, 0, 0, date_text, &Courier_New24, 1, COLORED);
    epd_SetFrameMemory(
        paint_GetImage(&paint),
        epd_GetWidth() - paint_GetWidth(&paint) - y,
        x,
        paint_GetWidth(&paint),
        paint_GetHeight(&paint)
    );
}


void show_alarm_bell(int x, int y, valarm_t * alarm) {
    const unsigned char * to_draw = 0;

    if (alarm->set) {
        to_draw = img_bell;
    }

    if (
        ! alarm->set &&
        (current_state == SET_ALARMS ||
        current_state == SET_ALARM_SET)
    ) {
        to_draw = img_bell_disabled;
    }

    if (alarm->active) {
        to_draw = img_bell_ringing;
    }

    if (to_draw) {
        paint_DrawImageAt(&paint, x, y, to_draw, 1, COLORED);
    }
}


void show_alarm_time(int x, int y, valarm_t * alarm) {
    char time_text[16];
    sprintf(
        time_text,
        "%02u:%02u",
        alarm->hour,
        alarm->minute
    );

    paint_DrawStringAt(&paint, x, y, time_text, &Courier_New16, 1, COLORED);
}


void show_alarm_dow(int x, int y, valarm_t * alarm) {
    const char dow[] = "MTWTFSS";

    paint_DrawStringAt(&paint, x, y, dow, &Courier_New16, 1, COLORED);
    for (int i = 0; i < 7; i++) {
        if (alarm->dow & (0x01 << (i + 1))) {
            paint_DrawFilledRectangle(
                &paint,
                x + i * Courier_New16.Width + 1,
                y + Courier_New16.Height - 3,
                x + i * Courier_New16.Width + Courier_New16.Width - 2,
                y + Courier_New16.Height - 2,
                COLORED
            );
        }
    }
}


void show_alarm_pointer() {
    int x, y, up;
    switch(current_state) {
        case SET_ALARM_SET:           x = 12;     y = 0;  up = 0; break;
        case SET_ALARM_HOUR1:         x = 18+8*1; y = 0;  up = 0; break;
        case SET_ALARM_HOUR2:         x = 18+8*2; y = 0;  up = 0; break;
        case SET_ALARM_MINUTE1:       x = 18+8*4; y = 0;  up = 0; break;
        case SET_ALARM_MINUTE2:       x = 18+8*5; y = 0;  up = 0; break;
        case SET_ALARM_DOW_MONDAY:    x = 18+8*1; y = 32; up = 1; break;
        case SET_ALARM_DOW_TUESDAY:   x = 18+8*2; y = 32; up = 1; break;
        case SET_ALARM_DOW_WEDNESDAY: x = 18+8*3; y = 32; up = 1; break;
        case SET_ALARM_DOW_THURSDAY:  x = 18+8*4; y = 32; up = 1; break;
        case SET_ALARM_DOW_FRIDAY:    x = 18+8*5; y = 32; up = 1; break;
        case SET_ALARM_DOW_SATURDAY:  x = 18+8*6; y = 32; up = 1; break;
        case SET_ALARM_DOW_SUNDAY:    x = 18+8*7; y = 32; up = 1; break;
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


//    0         1         2         3         4         5         6         7         8
//    01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678
//    ..........................................................................................
// 00 .                         │       ││       ││       ││       ││       │                  .
// 01 .                         │       ││       ││       ││       ││       │                  .
// 02 .                         │   █   ││   █   ││   █   ││   █   ││   █   │                  .
// 03 .       ╔      ██      ╗  │ ███   ││ ███   ││ ███   ││ ███   ││ ███   │                  .
// 04 .             █  █        │   █   ││   █   ││   █   ││   █   ││   █   │                  .
// 05 .             ████        │   █   ││   █   ││   █   ││   █   ││   █   │                  .
// 06 .            █    █       │   █   ││   █   ││   █   ││   █   ││   █   │                  .
// 07 .██         █      █      │   █   ││   █   ││   █   ││   █   ││   █   │                  .
// 08 .████       █      █      │   █   ││   █   ││   █   ││   █   ││   █   │                  .
// 09 .██████    █        █     ╟─  █  ─╢╟─  █  ─╢╟─  █  ─╢╟─  █  ─╢╟─  █  ─╢┌───────┐┌───────┐.
// 10 .████████  █        █     ║ █████ ║║ █████ ║║ █████ ║║ █████ ║║ █████ ║│       ││       │.
// 11 .██████    █        █     ║       ║║       ║║       ║║       ║║       ║│       ││       │.
// 12 .████      █        █     ║       ║║       ║║       ║║       ║║       ║│       ││       │.
// 13 .██       ██        ██    ███   ██║███   ██║███   ██║███   ██║███   ██║███   ██│███   ██│.
// 15 .        ██          ██   ╟██───██╢╟██───██╢╟██───██╢╟██───██╢╟██───██╢│██   ██││██   ██│.
// 16 .        █            █   │█ █ █ █││█ █ █ █││█ █ █ █││█ █ █ █││█ █ █ █││█ █ █ █││█ █ █ █│.
// 17 .         ████████████    │█ █ █ █││█ █ █ █││█ █ █ █││█ █ █ █││█ █ █ █││█ █ █ █││█ █ █ █│.
// 18 .             █  █        │█ █ █ █││█ █ █ █││█ █ █ █││█ █ █ █││█ █ █ █││█ █ █ █││█ █ █ █│.
// 18 .       ╚      ██      ╝  │█  █  █││█  █  █││█  █  █││█  █  █││█  █  █││█  █  █││█  █  █│.
// 19 .                         │█     █││█     █││█     █││█     █││█     █││█     █││█     █│.
// 20 .                         ███   ██│███   ██│███   ██│███   ██│███   ██│███   ██│███   ██│.
// 21 .                         │       ││       ││       ││       ││       ││       ││       │.
// 22 .                         │#######││#######││#######││#######││#######││#######││#######│.
// 23 .                         │#######││#######││#######││#######││#######││#######││#######│.
//    ..........................................................................................
//

// Old reference
//
//    0         1         2         3         4         5         6         7         8
//    012345678901234567890123456789012345678901234567890123456789012345678901234567890
//    ..................................................................................
// 00 .                         ┌─────┐┌─────┐┌─────┐┌─────┐┌─────┐                    .
// 01 .                         │     ││     ││     ││     ││     │                    .
// 02 .                         │     ││     ││     ││     ││     │                    .
// 03 .       ╔      ██      ╗  │ ██  ││ ██  ││     ││ ██  ││ ██  │                    .
// 04 .             █  █        │  █  ││  █  ││  █  ││  █  ││  █  │                    .
// 05 .             ████        │  █  ││  █  ││     ││  █  ││  █  │                    .
// 06 .            █    █       │  █  ││  █  ││     ││  █  ││  █  │                    .
// 07 .██         █      █      │  █  ││  █  ││  █  ││  █  ││  █  │                    .
// 08 .████       █      █      ╟█████╢╟█████╢╟─────╢╟█████╢╟█████╢┌─────┐┌─────┐      .
// 09 .██████    █        █     ║     ║║     ║║     ║║     ║║     ║│     ││     │      .
// 10 .████████  █        █     ║     ║║     ║║     ║║     ║║     ║│     ││     │      .
// 11 .██████    █        █     ██─███╢██─███╢██─███╢██─███╢██─███╢██ ███│██ ███│      .
// 12 .████      █        █     ██ ██ │██ ██ │██ ██ │██ ██ │██ ██ │██ ██ │██ ██ │      .
// 13 .██       ██        ██    ██ ██ │██ ██ │██ ██ │██ ██ │██ ██ │██ ██ │██ ██ │      .
// 15 .        ██          ██   █ █ █ │█ █ █ │█ █ █ │█ █ █ │█ █ █ │█ █ █ │█ █ █ │      .
// 16 .        █            █   █   █ │█   █ │█   █ │█   █ │█   █ │█   █ │█   █ │      .
// 17 .         ████████████    ██ ███│██ ███│██ ███│██ ███│██ ███│██ ███│██ ███│      .
// 18 .             █  █        │     ││     ││     ││     ││     ││     ││     │      .
// 18 .       ╚      ██      ╝  ######│######│######│######│######│######│######│      .
// 19 .                         ######┘######┘######┘######┘######┘######┘######┘      .
// 20 .                                                                                .
// 21 .                                                                                .
// 22 .                                                                                .
// 23 .                                                                                .
//    ..................................................................................
//

void _show_alarm(int x, int y, valarm_t * alarm, uint8_t i) {
    if (current_state == SET_ALARMS) {
        if (selected_alarm == i) {
            paint_DrawImageAt(&paint, x, y+7, img_arrow, 1, COLORED);
        }
    }

    show_alarm_bell(x + 8, y + 3, alarm);

    if (alarm->set) {
        show_alarm_time(x + 26, y-1, alarm);
        show_alarm_dow(x + 26, y + 9, alarm);
    }
}


void show_alarm(int x, int y, valarm_t * alarm, uint8_t i) {
    paint_SetWidth(&paint, 24);
    paint_SetHeight(&paint, 88);
    paint_Clear(&paint, UNCOLORED);

    _show_alarm(0, 0, alarm, i);

    epd_SetFrameMemory(
        paint_GetImage(&paint),
        epd_GetWidth() - paint_GetWidth(&paint) - y,
        x,
        paint_GetWidth(&paint),
        paint_GetHeight(&paint)
    );
}


//    0         1         2         3         4         5         6         7         8
//    01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678
//    ..........................................................................................
// 00 .           ███████      ███████  ███████           ███████  ███████                     .
// 01 .           ███████      ███████  ███████           ███████  ███████                     .
// 02 .            █████        █████    █████             █████    █████                      .
// 03 .            █████        █████    █████             █████    █████                      .
// 04 .             ███          ███      ███               ███      ███                       .
// 05 .             ███          ███      ███               ███      ███                       .
// 06 .              █            █        █                 █        █                        .
// 07 .              █        ┌───█───┐┌───█───┐┌───────┐┌───█───┐┌───█───┐                    .
// 08 .                       │       ││       ││       ││       ││       │                    .
// 09 .                       │       ││       ││       ││       ││       │                    .
// 10 .                       │   █   ││   █   ││       ││   █   ││   █   │                    .
// 11 .       ╔      ██      ╗│ ███   ││ ███   ││       ││ ███   ││ ███   │                    .
// 12 .             █  █      │   █   ││   █   ││  ██   ││   █   ││   █   │                    .
// 13 .             ████      │   █   ││   █   ││  ██   ││   █   ││   █   │                    .
// 14 .            █    █     │   █   ││   █   ││       ││   █   ││   █   │                    .
// 15 .██         █      █    │   █   ││   █   ││       ││   █   ││   █   │                    .
// 16 .████       █      █    │   █   ││   █   ││  ██   ││   █   ││   █   │                    .
// 17 .██████    █        █   ╟─  █  ─╢╟─  █  ─╢╟──██───╢╟─  █  ─╢╟─  █  ─╢┌───────┐┌───────┐  .
// 18 .████████  █        █   ║ █████ ║║ █████ ║║       ║║ █████ ║║ █████ ║│       ││       │  .
// 19 .██████    █        █   ║       ║║       ║║       ║║       ║║       ║│       ││       │  .
// 20 .████      █        █   ║       ║║       ║║       ║║       ║║       ║│       ││       │  .
// 21 .██       ██        ██  ███   ██║███   ██║███   ██║███   ██║███   ██║███   ██│███   ██│  .
// 22 .        ██          ██ ╟██───██╢╟██───██╢╟██───██╢╟██───██╢╟██───██╢│██   ██││██   ██│  .
// 23 .        █            █ │█ █ █ █││█ █ █ █││█ █ █ █││█ █ █ █││█ █ █ █││█ █ █ █││█ █ █ █│  .
// 24 .         ████████████  │█ █ █ █││█ █ █ █││█ █ █ █││█ █ █ █││█ █ █ █││█ █ █ █││█ █ █ █│  .
// 25 .             █  █      │█ █ █ █││█ █ █ █││█ █ █ █││█ █ █ █││█ █ █ █││█ █ █ █││█ █ █ █│  .
// 26 .       ╚      ██      ╝│█  █  █││█  █  █││█  █  █││█  █  █││█  █  █││█  █  █││█  █  █│  .
// 27 .                       │█     █││█     █││█     █││█     █││█     █││█     █││█     █│  .
// 28 .                       ███   ██│███   ██│███   ██│███   ██│███   ██│███   ██│███   ██│  .
// 29 .                       │       ││       ││       ││       ││       ││       ││       │  .
// 30 .                       │#######││#######││#######││#######││#######││#######││#######│  .
// 31 .                       │#######││#######││#######││#######││#######││#######││#######│  .
// 32 .                       └───█───┘└───█───┘└───█───┘└───█───┘└───█───┘└───█───┘└───█───┘  .
// 33 .                           █        █        █        █        █        █        █      .
// 34 .                          ███      ███      ███      ███      ███      ███      ███     .
// 35 .                          ███      ███      ███      ███      ███      ███      ███     .
// 36 .                         █████    █████    █████    █████    █████    █████    █████    .
// 37 .                         █████    █████    █████    █████    █████    █████    █████    .
// 38 .                        ███████  ███████  ███████  ███████  ███████  ███████  ███████   .
// 39 .                        ███████  ███████  ███████  ███████  ███████  ███████  ███████   .
// 40 .                                                                                        .
// 41 .                                                                                        .
// 42 .                                                                                        .
// 43 .                                                                                        .
// 44 .                                                                                        .
//    ..........................................................................................

void show_alarm_edit(int x, int y, valarm_t * alarm) {
    paint_SetWidth(&paint, 40);
    paint_SetHeight(&paint, 88);
    paint_Clear(&paint, UNCOLORED);

    _show_alarm(0, 8, alarm, 0);
    show_alarm_pointer();

    epd_SetFrameMemory(
        paint_GetImage(&paint),
        epd_GetWidth() - paint_GetWidth(&paint) - y - (-8),
        x,
        paint_GetWidth(&paint),
        paint_GetHeight(&paint)
    );
}


void view_init() {
    paint_init(&paint, canvas, 0, 0);
    paint_SetRotate(&paint, ROTATE_90);
}

// Display outline for default display
//
//       00112233445566778899aabbccddeef
//       0808080808080808080808080808080
//      .................................
// 0x00 .                               .
// 0x08 . ┌─────────────────┐           .
// 0x10 . │                 │┌─────────┐.
// 0x18 . │ time            ││ alarm1  │.
// 0x20 . │                 │└─────────┘.
// 0x28 . │                 │┌─────────┐.
// 0x30 . └─────────────────┘│ alarm2  │.
// 0x38 .                    └─────────┘.
// 0x40 .  ┌────────────────┐┌─────────┐.
// 0x48 .  │  dow           ││ alarm3  │.
// 0x50 .  └────────────────┘└─────────┘.
// 0x58 .  ┌────────────────┐┌─────────┐.
// 0x60 .  │  date          ││ alarm4  │.
// 0x68 .  └────────────────┘└─────────┘.
// 0x70 .                               .
// 0x78 .                               .
//      .................................
//
//

// Display outline for alarm edit display
//
//       00112233445566778899aabbccddeef
//       0808080808080808080808080808080
//      .................................
// 0x00 .                               .
// 0x08 . ┌─────────────────┐┌─────────┐.
// 0x10 . │                 ││         │.
// 0x18 . │ time            ││ alarm1  │.
// 0x20 . │                 ││         │.
// 0x28 . │                 │└─────────┘.
// 0x30 . └─────────────────┘           .
// 0x38 .                               .
// 0x40 .  ┌────────────────┐           .
// 0x48 .  │  dow           │           .
// 0x50 .  └────────────────┘           .
// 0x58 .  ┌────────────────┐           .
// 0x60 .  │  date          │           .
// 0x68 .  └────────────────┘           .
// 0x70 .                               .
// 0x78 .                               .
//      .................................
//
//

// Display outline for time edit display
//
//       00112233445566778899aabbccddeef
//       0808080808080808080808080808080
//      .................................
// 0x00 .                               .
// 0x08 . ┌─────────────────┐┌─────────┐.
// 0x10 . │                 ││         │.
// 0x18 . │ time            ││ alarm1  │.
// 0x20 . │                 ││         │.
// 0x28 . │                 │└─────────┘.
// 0x30 . └─────────────────┘           .
// 0x38 . ┌─pointer─────────┐           .
// 0x40 . └─────────────────┘           .
// 0x48 .                               .
// 0x50 .                               .
// 0x58 .                               .
// 0x60 .                               .
// 0x68 .                               .
// 0x70 .                               .
// 0x78 .                               .
//      .................................
//
//

// Display outline for DOW edit display
//
//       00112233445566778899aabbccddeef
//       0808080808080808080808080808080
//      .................................
// 0x00 .                               .
// 0x08 . ┌─────────────────┐┌─────────┐.
// 0x10 . │                 ││         │.
// 0x18 . │ time            ││ alarm1  │.
// 0x20 . │                 ││         │.
// 0x28 . │                 │└─────────┘.
// 0x30 . └─────────────────┘           .
// 0x38 .                               .
// 0x40 .  ┌────────────────┐           .
// 0x48 .>>│  dow           │           .
// 0x50 .  └────────────────┘           .
// 0x58 .                               .
// 0x60 .                               .
// 0x68 .                               .
// 0x70 .                               .
// 0x78 .                               .
//      .................................
//
//
// Display outline for date edit display
//
//       00112233445566778899aabbccddeef
//       0808080808080808080808080808080
//      .................................
// 0x00 .                               .
// 0x08 . ┌─────────────────┐┌─────────┐.
// 0x10 . │                 ││         │.
// 0x18 . │ time            ││ alarm1  │.
// 0x20 . │                 ││         │.
// 0x28 . │                 │└─────────┘.
// 0x30 . └─────────────────┘           .
// 0x38 .                               .
// 0x40 .  ┌────────────────┐           .
// 0x48 .  │  dow           │           .
// 0x50 .  └────────────────┘           .
// 0x58 .  ┌────────────────┐           .
// 0x60 .  │  date          │           .
// 0x68 .  └────────────────┘           .
// 0x70 .   ^ ^   ^ ^    ^ ^            .
// 0x78 .                               .
//      .................................
//
//

void show_dow_pointer(int x, int y) {
    paint_SetWidth(&paint, 8);
    paint_SetHeight(&paint, 16);
    paint_Clear(&paint, UNCOLORED);

    paint_DrawImageAt(&paint, 4, 0, img_arrow, 1, COLORED);
    epd_SetFrameMemory(
        paint_GetImage(&paint),
        epd_GetWidth() - paint_GetWidth(&paint) - y,
        x,
        paint_GetWidth(&paint),
        paint_GetHeight(&paint)
    );
}

void show_date_pointer(int epd_x, int epd_y) {
    int x, y;

    paint_SetWidth(&paint, 8);
    paint_SetHeight(&paint, 136);
    paint_Clear(&paint, UNCOLORED);

    switch(current_state) {
        case SET_DATE_DAY1:         x = 3 + 13*0; y = 0; break;
        case SET_DATE_DAY2:         x = 3 + 13*1; y = 0; break;
        case SET_DATE_MONTH1:       x = 3 + 13*3; y = 0; break;
        case SET_DATE_MONTH2:       x = 3 + 13*4; y = 0; break;
        case SET_DATE_YEAR1:        x = 3 + 13*8; y = 0; break;
        case SET_DATE_YEAR2:        x = 3 + 13*9; y = 0; break;
        default: x = 0; y = 0; break;
    }

    paint_DrawImageAt(&paint, x, y, img_arrow_up, 1, COLORED);
    epd_SetFrameMemory(
        paint_GetImage(&paint),
        epd_GetWidth() - paint_GetWidth(&paint) - epd_y,
        epd_x,
        paint_GetWidth(&paint),
        paint_GetHeight(&paint)
    );

}

void show_time_pointer(int epd_x, int epd_y) {
    int x, y;

    paint_SetWidth(&paint, 16);
    paint_SetHeight(&paint, 144);
    paint_Clear(&paint, UNCOLORED);

    switch(current_state) {
        case SET_DATE_HOUR1:         x = 6 + 29*0; y = 0; break;
        case SET_DATE_HOUR2:         x = 6 + 29*1; y = 0; break;
        case SET_DATE_MINUTE1:       x = 6 + 29*3; y = 0; break;
        case SET_DATE_MINUTE2:       x = 6 + 29*4; y = 0; break;
        default: x = 0; y = 0; break;
    }

    paint_DrawImageAt(&paint, x, y, img_arrow_up_large, 1, COLORED);
    epd_SetFrameMemory(
        paint_GetImage(&paint),
        epd_GetWidth() - paint_GetWidth(&paint) - epd_y,
        epd_x,
        paint_GetWidth(&paint),
        paint_GetHeight(&paint)
    );
}

void view_update(vdatetime_t * datetime, valarm_t * alarms) {
    if (
        current_state >= SET_DATE_HOUR1 &&
        current_state <= SET_DATE_YEAR2
    ) {
        datetime = &new_datetime;
    }

    show_time(0x08, 0x08, datetime);
    if (
        current_state >= SET_DATE_HOUR1 &&
        current_state <= SET_DATE_MINUTE2
    ) {
        show_time_pointer(0x08, 0x38);
    }

    if (
        current_state < SET_DATE_HOUR1 ||
        current_state > SET_DATE_MINUTE2
    ) {
        show_dow(0x10, 0x40, datetime);
        if (current_state == SET_DATE_DOW) {
            show_dow_pointer(0x00, 0x48);
        }
    }

    if (
        current_state < SET_DATE_HOUR1 ||
        current_state > SET_DATE_DOW
    ) {
        show_date(0x10, 0x58, datetime);
        if (
            current_state >= SET_DATE_DAY1 &&
            current_state <= SET_DATE_YEAR2
        ) {
            show_date_pointer(0x10, 0x70);
        }
    }

    if (
        current_state >= SET_ALARM_SET &&
        current_state <= SET_ALARM_DOW_SUNDAY
    ) {
        show_alarm_edit(0xa0, 0x10 + selected_alarm * 0x18, &new_alarm);
    } else {
        for (uint8_t i = 0; i < NUM_ALARMS; i++) {
            valarm_t * alarm = &alarms[i];
            show_alarm(0xa0, 0x10 + i*0x18, alarm, i);
        }
    }
}
