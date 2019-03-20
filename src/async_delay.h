#ifndef ASYNC_DELAY_H
#define ASYNC_DLEAY_H

#include <stdint.h>
#include "callbacks.h"


void async_delay_ms(
    uint16_t ms,
    cb_t _func,
    pctx_t func_ctx
);
void async_delay_stop();
void async_delay_trigger();


#endif // ASYNC_DELAY_H
