/**
 * Provides a shared list of states for the buttons and display to work with.
 */

#ifndef STATES_H
#define STATES_H

#include "datetime.h"
#include "alarms.h"

enum screen_state {
    DISPLAY,
    SET_ALARMS,
    SET_ALARM_HOUR1,
    SET_ALARM_HOUR2,
    SET_ALARM_MINUTE1,
    SET_ALARM_MINUTE2,
    SET_ALARM_DOW_MONDAY,
    SET_ALARM_DOW_TUESDAY,
    SET_ALARM_DOW_WEDNESDAY,
    SET_ALARM_DOW_THURSDAY,
    SET_ALARM_DOW_FRIDAY,
    SET_ALARM_DOW_SATURDAY,
    SET_ALARM_DOW_SUNDAY,
    SET_DATE_HOUR1,
    SET_DATE_HOUR2,
    SET_DATE_MINUTE_1,
    SET_DATE_MINUTE_2,
    SET_DATE_DOW,
    SET_DATE_DAY1,
    SET_DATE_DAY2,
    SET_DATE_MONTH1,
    SET_DATE_MONTH2,
    SET_DATE_YEAR1,
    SET_DATE_YEAR2,
};

extern enum screen_state current_state;
extern vdatetime_t new_datetime;
extern valarm_t new_alarm;
extern volatile uint8_t selected_alarm;
extern volatile uint8_t force_redraw;

void ui_init(
    vdatetime_t * _current_datetime,
    vdatetime_t * _last_datetime,
    valarm_t * _alarms
);
void ui_input(char);

#endif // STATES_H
