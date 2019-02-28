#ifndef TWI_H
#define TWI_H

#include<avr/io.h>

void twi_init(void);
void twi_start(void);
void twi_stop(void);
void twi_send(uint8_t data);
uint8_t twi_receive(uint8_t ack_val);

#endif