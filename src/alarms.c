#include <avr/eeprom.h>
#include "alarms.h"
#include "sounds.h"
#include "async_delay.h"


// Global variables stored in EEPROM
uint32_t EEMEM e_check_sum;
alarm_t EEMEM e_alarms[NUM_ALARMS];


// Checksum for EEPROM global variables
#define ALARM_CHECKSUM 0x2261cde8


void init_alarm(valarm_t * alarm) {
    alarm->set = 0;
    alarm->hour = 0;
    alarm->minute = 0;
    alarm->dow = 0;
}


void init_alarms(valarm_t * alarms) {
    uint32_t check_sum;

    check_sum = eeprom_read_dword(&e_check_sum);
    if (check_sum == ALARM_CHECKSUM) {
        for (int i = 0; i < NUM_ALARMS; i++) {
            eeprom_read_block((void *) &alarms[i], &e_alarms[i], sizeof(alarm_t));
        }
        return;
    }

    for (int i = 0; i < NUM_ALARMS; i++) {
        init_alarm(&alarms[i]);
        eeprom_write_block((void *) &alarms[i], &e_alarms[i], sizeof(alarm_t));
    }
    eeprom_write_dword(&e_check_sum, ALARM_CHECKSUM);
}


void save_alarm(valarm_t * alarm, uint8_t index) {
    eeprom_write_block((void *) alarm, &e_alarms[index], sizeof(alarm_t));
}


void save_alarms(valarm_t * alarms) {
    for (int i = 0; i < NUM_ALARMS; i++) {
        save_alarm(&alarms[i], i);
    }
}


uint8_t check_alarm(valarm_t * alarm, vdatetime_t * date) {
    if (!alarm->set) {
       return 0;
    }

    if (alarm->hour != date->hour) {
        return 0;
    }

    if (alarm->minute != date->minute) {
        return 0;
    }

    if (!(alarm->dow & (1<<date->dow))) {
        return 0;
    }

    alarm->active = -1;
    return -1;
}


uint8_t check_alarms(valarm_t * alarms, vdatetime_t * date) {
    for (int i = 0; i < NUM_ALARMS; i++) {
        if (check_alarm(&alarms[i], date)) {
            return -1;
        }
    }

    return 0;
}


uint8_t activated_alarms(valarm_t * alarms) {
    for (int i = 0; i < NUM_ALARMS; i++) {
        if (alarms[i].active) {
            return -1;
        }
    }

    return 0;
}


void clear_alarms(valarm_t * alarms) {
    for (int i = 0; i < NUM_ALARMS; i++) {
        alarms[i].active = 0;
    }
}


void copy_alarm(valarm_t * dest, valarm_t * src) {
    dest->set = src->set;
    dest->hour = src->hour;
    dest->minute = src->minute;
    dest->dow = src->dow;
    dest->active = src->active;
}
