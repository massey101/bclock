#include "tasks.h"

task_t tasks[NUM_TASKS] = {
    [TASK_TIMER] = &timer_task,
    [TASK_DISPLAY] = &display_task,
    [TASK_DISPLAY_SLEEP] = &display_sleep_task,
    [TASK_AUDIO] = &audio_task,
    [TASK_LED] = &led_task,
};
