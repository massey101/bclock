#ifndef DATETIME_H
#define DATETIME_H

#include <stdint.h>

typedef struct {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t dow;
} datetime_t;

typedef volatile datetime_t vdatetime_t;

extern const char * datetime_DOW[];

void datetime_copy(vdatetime_t * dest, vdatetime_t * src);

#endif // DATETIME_H
