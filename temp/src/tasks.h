#ifndef TASKS_H
#define TASKS_H

#include "reactor.h"

// Tasks with hard coded id's start at 128
enum tasks {
    TASK_PRINT = 0x80,
    TASK_FLASH_LED,
    TASK_BUTTONS,
};

#endif // TASKS_H
