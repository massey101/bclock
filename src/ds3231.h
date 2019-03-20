#ifndef DS3231_H
#define DS3231_H

#include "datetime.h"
#include "twi.h"

uint8_t bcd_to_dec(uint8_t bcd_val);
uint8_t dec_to_bcd(uint8_t dec_val);
void ds3231_init(void);
void ds3231_set(vdatetime_t * datetime);
void ds3231_get(vdatetime_t * datetime);

#endif
