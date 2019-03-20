#ifndef DATETIME_H
#define DATETIME_H

#include <stdint.h>


enum datetime_accuracy {
    SECONDS,
    MINUTES,
    HOURS,
    DAYS,
    MONTHS,
    YEARS,
    DOW,
};

typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint8_t year;
    uint8_t dow;
} datetime_t;

typedef volatile datetime_t vdatetime_t;

extern const char * datetime_DOW[];

/**
 * Deep copy a datetime object from src to dest.
 * src - source.
 * dest - dest.
 */
void datetime_copy(vdatetime_t * dest, vdatetime_t * src);


/**
 * Compare the equality of two datetimes to a certain accuracy.
 * Note - DOW accuracy is a full comparison of everything.
 *
 * date1, date2 - dates to compare
 * accuracy - The accuracy to compare to as specificed above
 */
uint8_t datetime_cmp(
    vdatetime_t * date1,
    vdatetime_t * date2,
    enum datetime_accuracy accuracy
);

#endif // DATETIME_H
