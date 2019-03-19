#ifndef ALARMS_H
#define ALARMS_H

#include <stdint.h>
#include "callbacks.h"
#include "ds3231.h"


#define NUM_ALARMS 4


typedef struct alarm {
    uint8_t set;
    uint8_t hour;
    uint8_t minute;
    uint8_t dow;
    uint8_t active;
} alarm_t;

typedef volatile alarm_t valarm_t;


/**
 * Initialise an empty alarm.
 *
 * alarm - The alarm to initialise
 */
void init_alarm(valarm_t * alarm);

/**
 * Initialise a number of alarms. If alarms are availalbe in the EEPROM then
 * they will be initialised from there.
 *
 * alarms - An array of NUM_ALARMS alarms to init
 */
void init_alarms(valarm_t * alarms);

/**
 * Check whether the alarm matches the date. If so the alarm will be activated.
 *
 * alarm - The alarm to check
 * date - Compare with this date
 * returns - True if the alarm matches the date
 */
uint8_t check_alarm(valarm_t * alarm, datetime_t * date);

/**
 * Check whether any alarms match the date. If any do they will be activated.
 *
 * alarms - An array of NUM_ALARMS alarms to check
 * date - Compare with this date
 * returns - True if any alarms match the date
 */
uint8_t check_alarms(valarm_t * alarms, datetime_t * date);

/**
 * Check whether any alarms have previously been activated by `check_alarm`.
 *
 * alarms - An array of NUM_ALARMS alarms to check
 * returns - True if any alarm is activated
 */
uint8_t activated_alarms(valarm_t * alarms);

/**
 * Deactive any alarms that are active.
 *
 * alarms - An array of NUM_ALARMS alarms to deactivate
 */
void clear_alarms(valarm_t * alarms);

/**
 * Use the pcm_audio module to start the alarm. This will stop when all the
 * alarms are cleared.
 *
 * alarms - An array of NUM_ALARMS alarms to watch for deactivation
 */
void start_alarm(valarm_t * alarms);

#endif // ALARMS_H
