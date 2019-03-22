// This code is designed to add uart printf functionality.
//
// In order to use it simply use:
//
//   uart_init(38400);
//   stdout = &uart_stdout;
//   stdin = &uart_stdin;
//
//   printf("Oh jeez Rick");
//

#ifndef UART_H
#define UART_H

#include <stdio.h>
#include <stdint.h>

FILE uart_stdout;
FILE uart_input;
FILE uart_io;

typedef void (*uart_cb_t)(char input);

void uart_init(uint32_t baud);
void uart_init_interrupt(uint32_t baud, uart_cb_t cb);
char uart_getc(void);
void uart_sendc(char data);
int uart_getchar(FILE * stream);
int uart_printchar(char var, FILE * stream);
void uart_flush();

#endif // UART_H
