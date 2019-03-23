#ifndef REACTOR_H
#define REACTOR_H

#include "sleep.h"

/**
 * All reactor callbacks should use this format.
 *
 * real_ms - The real number of milliseconds this task has slept for.
 */
typedef void (* volatile task_t)(ms_t real_ms);

/**
 * Call this task after x ms.
 *
 * name - A unique id for the task.
 * task - The task to call.
 * ms - The number of milliseconds to call the task in.
 *
 * returns - true on success and false on failure.
 */
uint8_t reactor_call_later(uint8_t id, task_t task, ms_t ms);

/**
 * Trigger a waiting task to be called immediately when the reactor is next
 * updated. Should be used in conjunction with `reactor_update()`.
 *
 * id - The unique id of the task to trigger.
 * ms - reset the trigger down to this if it is higher.
 *
 * returns - true on success and false on failure.
 */
uint8_t reactor_trigger(uint8_t id, ms_t ms);

/**
 * Cancel a waiting task.
 *
 * id - The unique id of the task to cancel.
 *
 * return - true if a task was cancelled and false if not.
 */
uint8_t reactor_cancel(uint8_t id);

/**
 * Force the reactor to wake up and update.
 */
void reactor_update();

/**
 * Stop the reactor from sleeping to do things like play audio.
 */
void reactor_disable_sleep();

/**
 * Re-enable the reactor sleeping.
 */
void reactor_enable_sleep();

/**
 * Start the reactor. Note: Never finishes.
 * Replaces the main while loop.
 */
void reactor();


#endif // REACTOR_H
