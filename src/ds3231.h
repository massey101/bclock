#ifndef DS3231_H
#define DS3231_H

#include "twi.h"

typedef struct {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t dow;
} datetime_t;

extern const char * datetime_DOW[];

uint8_t bcd_to_dec(uint8_t bcd_val);
uint8_t dec_to_bcd(uint8_t dec_val);
void ds3231_init(void);
void ds3231_set(datetime_t * datetime);
void ds3231_get(datetime_t * datetime);

#endif
