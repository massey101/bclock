#ifndef ASYNC_DELAY_H
#define ASYNC_DLEAY_H

#include <stdint.h>


void async_delay_ms(uint16_t ms, void * func_ctx, void (*func)(void * ctx));
void async_delay_stop();


#endif // ASYNC_DELAY_H
