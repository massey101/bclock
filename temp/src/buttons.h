#ifndef BUTTONS_H
#define BUTTONS_H

typedef void (*buttons_cb_t)(char input);

void buttons_init(buttons_cb_t _cb);

#endif // BUTTONS_H
