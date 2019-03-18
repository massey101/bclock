#ifndef ALARMS_H
#define ALARMS_H

#include <stdint.h>
#include "ds3231.h"


#define NUM_ALARMS 4


typedef struct alarm {
    uint8_t set;
    uint8_t hour;
    uint8_t minute;
    uint8_t dow;
    uint8_t active;
} alarm_t;


/**
 * Initialise an empty alarm.
 *
 * alarm - The alarm to initialise
 */
void init_alarm(alarm_t * alarm);

/**
 * Initialise a number of alarms. If alarms are availalbe in the EEPROM then
 * they will be initialised with those.
 *
 * alarms - An array of NUM_ALARMS alarms.
 */
void init_alarms(alarm_t * alarms);


uint8_t check_alarm(alarm_t * alarm, datetime_t * date);
uint8_t check_alarms(alarm_t * alarms, datetime_t * date);
uint8_t activated_alarms(alarm_t * alarms);
void clear_alarms(alarm_t * alarms);

#endif // ALARMS_H
