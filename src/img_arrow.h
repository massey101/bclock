#ifndef _IMG_ARROW_H_
#define _IMG_ARROW_H_

#include <avr/pgmspace.h>

static const uint8_t PROGMEM img_arrow[] = {
    0x08, 0x08,
    0xc0,
    0xf0,
    0xfc,
    0xff,
    0xfc,
    0xf0,
    0xc0,
    0x00,
};

#endif

// 11......
// 1111....
// 111111..
// 11111111
// 111111..
// 1111....
// 11......
// ........
