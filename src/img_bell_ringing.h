#ifndef _IMG_BELL_RINGING_H_
#define _IMG_BELL_RINGING_H_

#include <avr/pgmspace.h>

static const uint8_t PROGMEM img_bell_ringing[] = {
    0x10, 0x10,
    0x21, 0x84,
    0x4a, 0x52,
    0x93, 0xc9,
    0xa4, 0x25,
    0x28, 0x14,
    0x48, 0x12,
    0x50, 0x0a,
    0x50, 0x0a,
    0x50, 0x0a,
    0x90, 0x09,
    0x30, 0x0c,
    0x60, 0x06,
    0x40, 0x02,
    0x3f, 0xfc,
    0x02, 0x40,
    0x01, 0x80,
};

#endif

// ..1....11....1..
// .1..1.1..1.1..1.
// 1..1..1111..1..1
// 1.1..1....1..1.1
// ..1.1......1.1..
// .1..1......1..1.
// .1.1........1.1.
// .1.1........1.1.
// .1.1........1.1.
// 1..1........1..1
// ..11........11..
// .11..........11.
// .1............1.
// ..111111111111..
// ......1..1......
// .......11.......