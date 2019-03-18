#include <avr/eeprom.h>
#include "alarms.h"


#define ALARM_CHECKSUM 0x2261cde8


uint32_t EEMEM e_check_sum;
alarm_t EEMEM e_alarms[NUM_ALARMS];


void init_alarm(alarm_t * alarm) {
    alarm->set = 0;
    alarm->hour = 0;
    alarm->minute = 0;
    alarm->dow = 0;
}


void init_alarms(alarm_t * alarms) {
    uint32_t check_sum;

    DDRC |= (1<<PC0);

    check_sum = eeprom_read_dword(&e_check_sum);
    if (check_sum == ALARM_CHECKSUM) {
        for (int i = 0; i < NUM_ALARMS; i++) {
            eeprom_read_block(&alarms[i], &e_alarms[i], sizeof(alarm_t));
        }
        return;
    }

    for (int i = 0; i < NUM_ALARMS; i++) {
        init_alarm(&alarms[i]);
        eeprom_write_block(&alarms[i], &e_alarms[i], sizeof(alarm_t));
    }
    eeprom_write_dword(&e_check_sum, ALARM_CHECKSUM);
}


uint8_t check_alarm(alarm_t * alarm, datetime_t * date) {
    if (!alarm->set) {
       return 0;
    }

    if (alarm->hour != date->hours) {
        return 0;
    }

    if (alarm->minute != date->minutes) {
        return 0;
    }

    if (!(alarm->dow & (1<<date->dow))) {
        return 0;
    }

    alarm->active = -1;
    return -1;
}


uint8_t check_alarms(alarm_t * alarms, datetime_t * date) {
    for (int i = 0; i < NUM_ALARMS; i++) {
        if (check_alarm(&alarms[i], date)) {
            return -1;
        }
    }

    return 0;
}


uint8_t activated_alarms(alarm_t * alarms) {
    for (int i = 0; i < NUM_ALARMS; i++) {
        if (alarms[i].active) {
            return -1;
        }
    }

    return 0;
}
