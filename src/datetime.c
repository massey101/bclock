#include "datetime.h"

const char * datetime_DOW[] = {
    "",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday",
    "Sunday"
};


void datetime_copy(vdatetime_t * dest, vdatetime_t * src) {
    dest->hour = src->hour;
    dest->minute = src->minute;
    dest->second = src->second;
    dest->year = src->year;
    dest->month = src->month;
    dest->day = src->day;
    dest->dow = src->dow;
}

uint8_t datetime_cmp(
    vdatetime_t * date1,
    vdatetime_t * date2,
    enum datetime_accuracy accuracy
) {
    uint8_t retval = 1;

    switch (accuracy) {
        case DOW:
            if (date1->dow != date2->dow) {
                retval = 0;
            }
        case SECONDS:
            if (date1->second != date2->second) {
                retval = 0;
            }
        case MINUTES:
            if (date1->minute != date2->minute) {
                retval = 0;
            }
        case HOURS:
            if (date1->hour != date2->hour) {
                retval = 0;
            }
        case DAYS:
            if (date1->day != date2->day) {
                retval = 0;
            }
        case MONTHS:
            if (date1->month != date2->month) {
                retval = 0;
            }
        case YEARS:
            if (date1->year != date2->year) {
                retval = 0;
            }
            break;
        default:
            retval = 0;
    }

    return retval;
}
