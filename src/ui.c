#include "ui.h"
#include "view.h"
#include "ds3231.h"
#include "epd2in13.h"


#include <stdio.h>


enum screen_state current_state;
vdatetime_t new_datetime;
valarm_t new_alarm;
volatile uint8_t selected_alarm;
volatile uint32_t tone_debug_frequency;

static vdatetime_t * current_datetime;
static vdatetime_t * last_datetime;
static valarm_t * alarms;
void (*force_redraw_func)(uint8_t full_update);
void (*stop_alarm_func)();
void (*start_alarm_func)(uint32_t freq);


void ui_init(
    vdatetime_t * _current_datetime,
    vdatetime_t * _last_datetime,
    valarm_t * _alarms,
    void (*_force_redraw_func)(uint8_t full_update),
    void (*_stop_alarm_func)(),
    void (*_start_alarm_func)(uint32_t freq)
) {
    current_datetime = _current_datetime;
    last_datetime = _last_datetime;
    alarms = _alarms;
    current_state = DISPLAY;
    force_redraw_func= _force_redraw_func;
    stop_alarm_func = _stop_alarm_func;
    start_alarm_func = _start_alarm_func;
    tone_debug_frequency = 440000;
}


enum screen_state basic_input(
    volatile uint8_t * dest,
    uint8_t mult,
    uint8_t min,
    uint8_t max,
    char input
) {
    int16_t candidate = *dest;
    uint8_t orig_dest = *dest;
    switch(input) {
        case 'L':
            current_state -= 1;
            break;
        case 'U':
            candidate += mult;
            if (candidate > max) {
                candidate = max;
            }

            *dest = candidate;
            break;
        case 'D':
            candidate -= mult;
            if (candidate < min) {
                candidate = min;
            }

            *dest = candidate;
            break;
        case 'R':
            current_state += 1;
            break;
        default:
            break;
    }

    if (orig_dest != *dest) {
        return 1;
    }

    return 0;
}


enum screen_state bool_input(volatile uint8_t * dest, uint8_t offset, uint8_t input) {
    uint8_t orig_dest = *dest;
    switch(input) {
        case 'L':
            current_state -= 1;
            break;
        case 'U':
            *dest |= (1 << offset);
            break;
        case 'D':
            *dest &= ~(1 << offset);
            break;
        case 'R':
            current_state += 1;
            break;
        default:
            break;
    }

    if (orig_dest != *dest) {
        return 1;
    }

    return 0;
}


// Returns whether the screen needs to be redrawn
int ui_input(char input) {
    uint8_t last_state = current_state;
    uint8_t ui_force_redraw = 0;

    switch(current_state) {

        case TONE_DEBUG:
            switch(input) {
                case 'R':
                    tone_debug_frequency += 10000;
                    printf("t%lu\n", (unsigned long) tone_debug_frequency);
                    break;
                case 'L':
                    tone_debug_frequency -= 10000;
                    printf("t%lu\n", (unsigned long) tone_debug_frequency);
                    break;
                case 'U':
                    printf("st%lu\n", (unsigned long) tone_debug_frequency);
                    start_alarm_func(tone_debug_frequency);
                    break;
                case 'D':
                    printf("ed\n");
                    stop_alarm_func();
                    current_state = DISPLAY;
                    break;
            }
            break;

        case DISPLAY:
            switch(input) {
                case 'R':
                    selected_alarm = 0;
                    current_state = SET_ALARMS;
                    break;
                case 'L':
                    datetime_copy(&new_datetime, current_datetime);
                    new_datetime.second = 0;
                    current_state = SET_DATE_HOUR1;
                    break;
                case 'U':
                    current_state = TONE_DEBUG;
                    printf("t%lu\n", (unsigned long) tone_debug_frequency);
                    break;
                case 'D':
                    printf("stop\n");
                    stop_alarm_func();
                    ui_force_redraw = 1;
                    break;
            }
            break;


        case SET_ALARMS:
            switch(input) {
                case 'L':
                    current_state = DISPLAY;
                    break;
                case 'U':
                    if (selected_alarm > 0) {
                        selected_alarm -= 1;
                    }

                    ui_force_redraw = 1;
                    break;
                case 'D':
                    if (selected_alarm < NUM_ALARMS-1) {
                        selected_alarm += 1;
                    }

                    ui_force_redraw = 1;
                    break;
                case 'R':
                    copy_alarm(&new_alarm, &alarms[selected_alarm]);
                    current_state = SET_ALARM_SET;
                    break;
            }
            break;

        case SET_ALARM_SET:
            switch(input) {
                case 'L':
                    current_state = SET_ALARMS;
                    break;
                case 'U':
                    new_alarm.set = 1;
                    ui_force_redraw = 1;
                    break;
                case 'D':
                    new_alarm.set = 0;
                    ui_force_redraw = 1;
                    break;
                case 'R':
                    if (new_alarm.set) {
                        current_state = SET_ALARM_HOUR1;
                    } else {
                        copy_alarm(&alarms[selected_alarm], &new_alarm);
                        save_alarm(&alarms[selected_alarm], selected_alarm);

                        current_state = DISPLAY;
                    }
                    break;
            }
            break;


        case SET_ALARM_HOUR1:
            ui_force_redraw = basic_input(&new_alarm.hour, 10, 0, 24, input);
            break;
        case SET_ALARM_HOUR2:
            ui_force_redraw = basic_input(&new_alarm.hour, 1, 0, 24, input);
            break;
        case SET_ALARM_MINUTE1:
            ui_force_redraw = basic_input(&new_alarm.minute, 10, 0, 60, input);
            break;
        case SET_ALARM_MINUTE2:
            ui_force_redraw = basic_input(&new_alarm.minute, 1, 0, 60, input);
            break;
        case SET_ALARM_DOW_MONDAY:
            ui_force_redraw = bool_input(&new_alarm.dow, 1, input);
            break;
        case SET_ALARM_DOW_TUESDAY:
            ui_force_redraw = bool_input(&new_alarm.dow, 2, input);
            break;
        case SET_ALARM_DOW_WEDNESDAY:
            ui_force_redraw = bool_input(&new_alarm.dow, 3, input);
            break;
        case SET_ALARM_DOW_THURSDAY:
            ui_force_redraw = bool_input(&new_alarm.dow, 4, input);
            break;
        case SET_ALARM_DOW_FRIDAY:
            ui_force_redraw = bool_input(&new_alarm.dow, 5, input);
            break;
        case SET_ALARM_DOW_SATURDAY:
            ui_force_redraw = bool_input(&new_alarm.dow, 6, input);
            break;
        case SET_ALARM_DOW_SUNDAY:
            ui_force_redraw = bool_input(&new_alarm.dow, 7, input);
            if (current_state == SET_ALARM_DOW_SUNDAY + 1) {
                copy_alarm(&alarms[selected_alarm], &new_alarm);
                save_alarm(&alarms[selected_alarm], selected_alarm);

                current_state = DISPLAY;
            }
            break;

        case SET_DATE_HOUR1:
            ui_force_redraw = basic_input(&new_datetime.hour, 10, 0, 24, input);
            if (current_state == SET_DATE_HOUR1 - 1) {
                current_state = DISPLAY;
            }
            break;
        case SET_DATE_HOUR2:
            ui_force_redraw = basic_input(&new_datetime.hour, 1, 0, 24, input);
            break;
        case SET_DATE_MINUTE1:
            ui_force_redraw = basic_input(&new_datetime.minute, 10, 0, 60, input);
            break;
        case SET_DATE_MINUTE2:
            ui_force_redraw = basic_input(&new_datetime.minute, 1, 0, 60, input);
            break;
        case SET_DATE_DOW:
            ui_force_redraw = basic_input(&new_datetime.dow, 1, 1, 7, input);
            break;
        case SET_DATE_DAY1:
            ui_force_redraw = basic_input(&new_datetime.day, 10, 1, 31, input);
            break;
        case SET_DATE_DAY2:
            ui_force_redraw = basic_input(&new_datetime.day, 1, 1, 31, input);
            break;
        case SET_DATE_MONTH1:
            ui_force_redraw = basic_input(&new_datetime.month, 10, 1, 12, input);
            break;
        case SET_DATE_MONTH2:
            ui_force_redraw = basic_input(&new_datetime.month, 1, 1, 12, input);
            break;
        case SET_DATE_YEAR1:
            ui_force_redraw = basic_input(&new_datetime.year, 10, 0, 99, input);
            break;
        case SET_DATE_YEAR2:
            ui_force_redraw = basic_input(&new_datetime.year, 1, 0, 99, input);
            if (current_state == SET_DATE_YEAR2 + 1) {
                ds3231_set(&new_datetime);
                datetime_copy(last_datetime, current_datetime);
                datetime_copy(current_datetime, &new_datetime);

                current_state = DISPLAY;
            }
            break;
    }

    if (current_state != last_state) {
        force_redraw_func(0);

        if (ui_force_redraw == 0) {
            ui_force_redraw = 1;
        }
    }

    return ui_force_redraw;
}
