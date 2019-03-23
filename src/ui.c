#include "ui.h"
#include "view.h"
#include "ds3231.h"
#include "epd2in13.h"


#include <stdio.h>


enum screen_state current_state;
vdatetime_t new_datetime;
valarm_t new_alarm;
volatile uint8_t selected_alarm;

static vdatetime_t * current_datetime;
static vdatetime_t * last_datetime;
static valarm_t * alarms;
void (*force_redraw_func)(uint8_t full_update);
void (*stop_alarm_func)();


void ui_init(
    vdatetime_t * _current_datetime,
    vdatetime_t * _last_datetime,
    valarm_t * _alarms,
    void (*_force_redraw_func)(uint8_t full_update),
    void (*_stop_alarm_func)()
) {
    current_datetime = _current_datetime;
    last_datetime = _last_datetime;
    alarms = _alarms;
    current_state = DISPLAY;
    force_redraw_func= _force_redraw_func;
    stop_alarm_func = _stop_alarm_func;
}


void set_digit(volatile uint8_t * dest, uint8_t offset, uint8_t value) {
    value -= '0';
    if (offset == 0) {
        *dest = 10*((*dest) / 10) + value;
    }
    if (offset == 1) {
        *dest = 10 * value + (*dest) % 10;
    }
}


void set_bool(volatile uint8_t * dest, uint8_t offset, uint8_t value) {
    value -= '0';
    if (value) {
        *dest |= (1 << offset);
    } else {
        *dest &= ~(1 << offset);
    }
}


enum screen_state basic_input(volatile uint8_t * dest, uint8_t offset, uint8_t input) {
    switch(input) {
        case '#':
            return DISPLAY;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            set_digit(dest, offset, input);
        case '*':
            return current_state + 1;
        default:
            return current_state;
    }
}

enum screen_state bool_input(volatile uint8_t * dest, uint8_t offset, uint8_t input) {
    switch(input) {
        case '#':
            return DISPLAY;
        case '0':
        case '1':
            set_bool(dest, offset, input);
        case '*':
            return current_state + 1;
        default:
            return current_state;
    }
}


enum screen_state multi_input(
    volatile uint8_t * dest,
    uint8_t min,
    uint8_t max,
    uint8_t input
) {
    switch(input) {
        case '#':
            return DISPLAY;
        case '4':
            if (*dest < max) {
                *dest = (*dest)++;
            }
            return current_state;
        case '7':
            if (*dest > min) {
                *dest = (*dest)--;
            }
            return current_state;
        case '*':
            return current_state + 1;
        default:
            return current_state;
    }
}


void ui_input(char input) {
    uint8_t last_state = current_state;
    uint8_t ui_force_redraw = 0;

    printf("In: %c", input);

    if (input == 's') {
        printf(" Stopping alarm");
        stop_alarm_func();
        ui_force_redraw = 1;
    }

    switch(current_state) {

        case DISPLAY:
            switch(input) {
                case '1':
                    selected_alarm = 0;
                    current_state = SET_ALARMS;
                    break;
                case '2':
                    datetime_copy(&new_datetime, current_datetime);
                    new_datetime.second = 0;
                    current_state = SET_DATE_HOUR1;
                    break;
                case 'r':
                    printf(" Setting force_redraw");
                    current_state = current_state;
                    ui_force_redraw = 2;
                    break;
            }
            break;


        case SET_ALARMS:
            switch(input) {
                case '#':
                    current_state = DISPLAY;
                    break;
                case '1':
                case '2':
                case '3':
                case '4':
                    selected_alarm = input - '1';
                    copy_alarm(&new_alarm, &alarms[selected_alarm]);
                    new_alarm.set = 1;
                    current_state = SET_ALARM_HOUR1;
                    break;
            }
            break;


        case SET_ALARM_HOUR1:
            current_state = basic_input(&new_alarm.hour, 1, input);
            break;
        case SET_ALARM_HOUR2:
            current_state = basic_input(&new_alarm.hour, 0, input);
            break;
        case SET_ALARM_MINUTE1:
            current_state = basic_input(&new_alarm.minute, 1, input);
            break;
        case SET_ALARM_MINUTE2:
            current_state = basic_input(&new_alarm.minute, 0, input);
            break;
        case SET_ALARM_DOW_MONDAY:
            current_state = bool_input(&new_alarm.dow, 1, input);
            break;
        case SET_ALARM_DOW_TUESDAY:
            current_state = bool_input(&new_alarm.dow, 2, input);
            break;
        case SET_ALARM_DOW_WEDNESDAY:
            current_state = bool_input(&new_alarm.dow, 3, input);
            break;
        case SET_ALARM_DOW_THURSDAY:
            current_state = bool_input(&new_alarm.dow, 4, input);
            break;
        case SET_ALARM_DOW_FRIDAY:
            current_state = bool_input(&new_alarm.dow, 5, input);
            break;
        case SET_ALARM_DOW_SATURDAY:
            current_state = bool_input(&new_alarm.dow, 6, input);
            break;
        case SET_ALARM_DOW_SUNDAY:
            current_state = bool_input(&new_alarm.dow, 7, input);
            if (current_state == SET_ALARM_DOW_SUNDAY + 1) {
                copy_alarm(&alarms[selected_alarm], &new_alarm);
                save_alarm(&alarms[selected_alarm], selected_alarm);

                current_state = DISPLAY;
            }
            break;

        case SET_DATE_HOUR1:
            current_state = basic_input(&new_datetime.hour, 1, input);
            break;
        case SET_DATE_HOUR2:
            current_state = basic_input(&new_datetime.hour, 0, input);
            break;
        case SET_DATE_MINUTE_1:
            current_state = basic_input(&new_datetime.minute, 1, input);
            break;
        case SET_DATE_MINUTE_2:
            current_state = basic_input(&new_datetime.minute, 0, input);
            break;
        case SET_DATE_DOW:
            current_state = multi_input(&new_datetime.dow, 1, 7, input);
            break;
        case SET_DATE_DAY1:
            current_state = basic_input(&new_datetime.day, 1, input);
            break;
        case SET_DATE_DAY2:
            current_state = basic_input(&new_datetime.day, 0, input);
            break;
        case SET_DATE_MONTH1:
            current_state = basic_input(&new_datetime.month, 1, input);
            break;
        case SET_DATE_MONTH2:
            current_state = basic_input(&new_datetime.month, 0, input);
            break;
        case SET_DATE_YEAR1:
            current_state = basic_input(&new_datetime.year, 1, input);
            break;
        case SET_DATE_YEAR2:
            current_state = basic_input(&new_datetime.year, 0, input);
            if (current_state == SET_DATE_YEAR2 + 1) {
                ds3231_set(&new_datetime);
                datetime_copy(last_datetime, current_datetime);
                datetime_copy(current_datetime, &new_datetime);

                current_state = DISPLAY;
            }
            break;
    }

    if (ui_force_redraw) {
        force_redraw_func(ui_force_redraw - 1);
        return;
    }

    if (current_state != last_state) {
        printf(" %d -> %d", input, current_state);
        force_redraw_func(0);
    }
    printf("\n");
}
