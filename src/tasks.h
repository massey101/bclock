#ifndef TASKS_H
#define TASKS_H

#include "reactor.h"

// Tasks with hard coded id's start at 128
enum tasks {
    TASK_TIMER = 0x80,
    TASK_AUDIO,
    TASK_DISPLAY,
    TASK_DISPLAY_SLEEP,
};

#endif // TASKS_H
