#include "twi.h"

void twi_init(void)
{
    TWSR=0x00;
    TWBR=0x0F;//see how to configure clock freq of twi.here the freq for 16mhz xtal is nearabout 347khz
    TWCR =(1<<TWEN);//enable TWEN bit
}

void twi_start(void)
{
    TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);//TWINT=0 by writting one to it,TWSTA bit=1,TWEN bit=1 to transmit start condition when the bus is free.
    while((TWCR & (1<<TWINT))==0);//stay here and poll until TWINT becomes 1 at the end of transmit.
}

void twi_stop(void)
{
    TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//TWINT=0 by writting one to it,TWSTO bit=1,TWEN bit=1 to transmit stOP condition.
}

void twi_send(uint8_t data)
{
    TWDR=data;//send the data to TWDR resistor
    TWCR=(1<<TWINT)|(1<<TWEN);//TWINT=0 by writting one to it,TWEN bit=1 to enable twi module
    while((TWCR & (1<<TWINT))==0);//stay here and poll until TWINT becomes 1 at the end of transmit.
}

uint8_t twi_receive(uint8_t ack_val)
{
    TWCR=(1<<TWINT)|(1<<TWEN)|(ack_val<<TWEA);//if we want to receive more than one byte,we will send 1 as ack_val to send an acknowledge.At the time of last by te,we will send 0 as ack_val to send NACK.
    while((TWCR & (1<<TWINT))==0);//stay here and poll until TWINT becomes 1 at the end of transmit.
    return TWDR;
}
