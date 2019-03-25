#ifndef _IMG_BELL_DISABLED_H_
#define _IMG_BELL_DISABLED_H_

#include <avr/pgmspace.h>

static const uint8_t PROGMEM img_bell_disabled[] = {
    0x10, 0x10,
    0x01, 0x81,
    0x02, 0x42,
    0x03, 0xc4,
    0x04, 0x08,
    0x08, 0x10,
    0x08, 0x20,
    0x10, 0x48,
    0x10, 0x88,
    0x11, 0x08,
    0x12, 0x08,
    0x24, 0x0c,
    0x48, 0x06,
    0x10, 0x02,
    0x27, 0xfc,
    0x42, 0x40,
    0x81, 0x80,
};

#endif

// .......11......1
// ......1..1....1.
// ......1111...1..
// .....1......1...
// ....1......1....
// ....1.....1.....
// ...1.....1..1...
// ...1....1...1...
// ...1...1....1...
// ...1..1.....1...
// ..1..1......11..
// .1..1........11.
// ...1..........1.
// ..1..111111111..
// .1....1..1......
// 1......11.......
