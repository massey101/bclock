#ifndef ASYNC_DELAY_H
#define ASYNC_DLEAY_H

#include <stdint.h>

typedef void (* volatile async_delay_cb_t)(uint32_t real_ms);


void async_delay_ms(
    uint32_t ms,
    async_delay_cb_t _func
);
void async_delay_stop();
void async_delay_trigger();


#endif // ASYNC_DELAY_H
