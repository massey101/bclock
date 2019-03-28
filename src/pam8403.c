#include <avr/io.h>
#include "pam8403.h"


void pam8403_init() {
    DDRD |= _BV(PD2);
}


void pam8403_enable() {
    PORTD |= _BV(PD2);
}


void pam8403_disable() {
    PORTD &= ~_BV(PD2);
}
