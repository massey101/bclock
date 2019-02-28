//This library contains the functions regarding ds3231.In order to function properly,twi core functions library must be included

#include "ds3231.h"

#define DS3231_ADDR 0xD0
#define DS3231_WR 0x00
#define DS3231_RD 0x01


uint8_t bcd_to_dec(uint8_t bcd_val) {
    return (((bcd_val >> 4) * 10) + (bcd_val & 0x0f));
}


uint8_t dec_to_bcd(uint8_t dec_val) {
    return (((dec_val / 10) << 4) + (dec_val % 10));
}


void ds3231_init(void) {
    twi_init();
    twi_start();
    // Set register 0x0E (The control register to 0x00 to enable the 1Hx square
    // wave at the SQW pin.
    twi_send(DS3231_ADDR | DS3231_WR);
    twi_send(0x0E);
    twi_send(0x00);
    twi_stop();
}


void ds3231_set(datetime_t * datetime) {
    uint8_t regs[7];
    regs[0] = dec_to_bcd(datetime->seconds);
    regs[1] = dec_to_bcd(datetime->minutes);
    regs[2] = dec_to_bcd(datetime->hours);
    regs[3] = datetime->dow;
    regs[4] = dec_to_bcd(datetime->day);
    regs[5] = dec_to_bcd(datetime->month);

    // There is a single bit in the month register for the centuary. We use
    // this to differentiate between the 1900s and 2000s.
    if (datetime->year >= 2000) {
        regs[5] |= 0x80;
        regs[6] = dec_to_bcd((datetime->year - 2000) & 0xff);
    } else {
        regs[6] = dec_to_bcd((datetime->year - 1900) & 0xff);
    }

    // Start twi in write mode and update from register address 0x00.
    twi_start();
    twi_send(DS3231_ADDR | DS3231_WR);
    twi_send(0x00);
    for (int i = 0; i < 7; i++) {
        twi_send(regs[i]);
    }

    twi_stop();
}


void ds3231_get(datetime_t * datetime) {
    uint8_t regs[7];

    // Start twi in write mode at register address 0x00 so we can read starting
    // from there in a moment.
    twi_start();
    twi_send(DS3231_ADDR | DS3231_WR);
    twi_send(0x00);

    // Read 7 registers and send nack on our last read.
    twi_start();
    twi_send(DS3231_ADDR | DS3231_RD);
    for (int i = 0; i < 7; i++) {
        uint8_t more = (i == 6) ? 0 : 1;
        regs[i] = twi_receive(more);
    }
    twi_stop();

    datetime->seconds = bcd_to_dec(regs[0]);
    datetime->minutes = bcd_to_dec(regs[1]);
    datetime->hours = bcd_to_dec(regs[2]);
    datetime->dow = regs[3];
    datetime->day= bcd_to_dec(regs[4]);
    datetime->month = bcd_to_dec(regs[5] & 0x7f);
    if (regs[5] & 0xf0) {
        datetime->year = 2000;
    } else {
        datetime->year = 1900;
    }
    datetime->year += bcd_to_dec(regs[6]);
}

