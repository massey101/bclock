#ifndef TASKS_H
#define TASKS_H

#include "sleep.h"

/**
 * All reactor callbacks should use this task format.
 *
 * real_ms - The real number of milliseconds this task has slept for.
 */
typedef void (* volatile task_t)(ms_t real_ms);

// List of tasks and task ids
enum task_id {
    TASK_TIMER,
    TASK_AUDIO,
    TASK_DISPLAY,
    TASK_DISPLAY_SLEEP,
    TASK_LED,

    NUM_TASKS,
};

void timer_task(ms_t real_ms);
void display_task(ms_t real_ms);
void display_sleep_task(ms_t real_ms);
void audio_task(ms_t real_ms);
void led_task(ms_t real_ms);

extern task_t tasks[NUM_TASKS];

#endif // TASKS_H
