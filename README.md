# bclock

BClock is a Low powered alarm clock for Barrukka unpacked challenge. The idea
for the project came from when I wanted to move my phone alarm so that I would
have to get out of bed to turn it off, however, then I would be unable to
charge my phone at night.

Goals:
  * Battery powered with a long life (>1 month).
  * Loud enough to wake someone up.
  * No annoying lights that make it difficult to sleep.
  * Be cool

## Progress
One battery powered clock on a breadboard with an epaper display, a RTC, and a very quiet speaker. Uses a full 3mA which gives about 1 month per AA sized 2000mAh Ni-MH battery.

## TODO

Goals:

  * Add buttons to set time and alarms
  * Amp circuit
  * Manufacture on veroboard
  * Case to contain the board in
  * Reduce power consumption further

Stretch Goals:

  * SD card music library
  * Manufacture on PCB
  * Usage documentation
  * The ability to detect low battery (kind of important)

## Dependencies

  * avr-gcc
    - avr-binutils
  * avr-libc
  * avr-libc-doc
  * avrdude
    - libftdi

## Authors

Jack Massey (jacknmassey@gmail.com)
