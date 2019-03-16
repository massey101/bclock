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

