#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "uart.h"

FILE uart_stdout = FDEV_SETUP_STREAM(
    uart_printchar,
    NULL,
    _FDEV_SETUP_WRITE
);

FILE uart_input = FDEV_SETUP_STREAM(
    NULL,
    uart_getchar,
    _FDEV_SETUP_READ
);

FILE uart_io = FDEV_SETUP_STREAM(
    uart_printchar,
    uart_getchar,
    _FDEV_SETUP_RW
);


static uart_cb_t g_cb;

void uart_init(uint32_t baud) {
    /* Set Baud rate */
    UBRR0 = F_CPU/16/baud-1;
    /* Set frame format: 8 bit data */
    UCSR0C = (3<<UCSZ00);
    /* Enable The receiver and transmitter and the interrupt */
    UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
}


void uart_init_interrupt(uint32_t baud, uart_cb_t cb) {
    uart_init(baud);

    g_cb = cb;

    /* Enable the RX interrupt */
    UCSR0B |= (1<<RXCIE0);
    sei();
}


ISR(USART_RX_vect) {
    /* You must read the value of UDR0 in the interrupt to clear it. */
    char data = UDR0;

    /* Ensure we have a callback function */
    if (g_cb != 0) {
        g_cb(data);
    }
}


char uart_getc()
{
    /* Wait untill a data is available */
    while (!(UCSR0A & (1<<RXC0)));
    /* Return data */
    return UDR0;
}


void uart_sendc(char data)
{
    UCSR0A &= (1<<TXC0);
    /* Wait untill the transmitter is ready */
    while (!(UCSR0A & (1<<UDRE0)));
    /* Send Data */
    UDR0 = data;
}


int uart_getchar(FILE * stream) {
    char c = uart_getc();
    /* Give return line endings. */
    if (c == '\r') {
        return '\n';
    }
    return c;
}


int uart_printchar(char var, FILE * stream)
{
    /* Give return line endings. */
    if (var == '\n') {
        uart_sendc('\r');
    }
    /* Send character */
    uart_sendc(var);
    return 0;
}

void uart_flush() {
    /* Wait untill the transmitter is ready */
    uart_sendc(0);
    while (!(UCSR0A & (1<<TXC0)));
    UCSR0A |= (1<<TXC0);
}
