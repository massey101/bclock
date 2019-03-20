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
#include "callbacks.h"

FILE uart_stdout;
FILE uart_input;
FILE uart_io;

typedef struct {
    char data;
    pctx_t ctx;
} uart_ctx_t;

typedef volatile uart_ctx_t vuart_ctx_t;


void uart_init(uint32_t baud);
void uart_init_interrupt(uint32_t baud, cb_t cb, vuart_ctx_t * ctx);
char uart_getc(void);
void uart_sendc(char data);
int uart_getchar(FILE * stream);
int uart_printchar(char var, FILE * stream);

#endif // UART_H
