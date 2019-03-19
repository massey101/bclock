#ifndef CALLBACKS_H
#define CALLBACKS_H

#define async_ctx volatile
typedef volatile void * volatile pctx_t;
typedef void (* volatile cb_t)(pctx_t);


#endif // CALLBACKS_H
