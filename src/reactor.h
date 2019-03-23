#ifndef REACTOR_H
#define REACTOR_H

#include "tasks.h"
#include "sleep.h"

/**
 * Call this task after x ms.
 *
 * id - A unique id for the task.
 * ms - The number of milliseconds to call the task in.
 */
void reactor_call_later(enum task_id id, ms_t ms);

/**
 * Cancel a waiting task.
 *
 * id - The unique id of the task to cancel.
 */
void reactor_cancel(enum task_id id);

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
