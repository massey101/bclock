#ifndef ASYNC_DELAY_H
#define ASYNC_DELAY_H

#include <stdint.h>

typedef int32_t ms_t;

/**
 * Enter low power mode.
 *
 * ms - Wake up after this many milliseconds.
 *
 * returns - The number of milliseconds we actually slept for.
 */
ms_t sleep_ms(ms_t ms);

/**
 * Wake up from sleep mode. (Technically we have to be already awake to even be
 * able to call this so it really means just don't go back to sleep.)
 */
void sleep_wakeup();

/**
 * Disable the sleep function from entering power save mode. Useful for things
 * like playing audio.
 */
void sleep_power_save_disable();

/**
 * Re-enable the sleep function entering power save mode.
 */
void sleep_power_save_enable();


#endif // ASYNC_DELAY_H
