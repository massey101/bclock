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
    dest->hours = src->hours;
    dest->minutes = src->minutes;
    dest->seconds = src->seconds;
    dest->year = src->year;
    dest->month = src->month;
    dest->day = src->day;
    dest->dow = src->dow;
}
