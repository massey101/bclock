#include <string.h>
#include <limits.h>
#include "reactor.h"

#define TASK_HEAP_SIZE 8

struct task {
    uint8_t active;
    uint8_t id;
    task_t task;
    ms_t ms;
    ms_t real_ms;
};


// The tasks are simply stored in a heap. Tasks not marked as active are
// ignored and can be replaced.
static volatile struct task task_heap[TASK_HEAP_SIZE] = {{0}};


uint8_t reactor_call_later(uint8_t id, task_t task, ms_t ms) {

    // Find the first empty place in the heap and insert the teask.
    for (int i = 0; i < TASK_HEAP_SIZE; i++) {
        if (task_heap[i].active) {
            continue;
        }

        task_heap[i].active = 1;
        task_heap[i].id = id;
        task_heap[i].task = task;
        task_heap[i].ms = ms;
        task_heap[i].real_ms = 0;
        return 0;
    }

    return 1;
}


uint8_t reactor_trigger(uint8_t id, ms_t ms) {
    // Search the heap for a task with a matching id and jump the delay until
    // the task is executed all the way down to 0.
    for (int i = 0; i < TASK_HEAP_SIZE; i++) {
        if (! task_heap[i].active) {
            continue;
        }

        if (task_heap[i].id != id) {
            continue;
        }

        if (task_heap[i].ms > ms) {
            task_heap[i].ms = ms;
        }

        return 0;
    }

    return 1;
}


uint8_t reactor_cancel(uint8_t id) {
    // Search the heap for a task with a matching id and deactivate it.
    for (int i = 0; i < TASK_HEAP_SIZE; i++) {
        if (! task_heap[i].active) {
            continue;
        }

        if (task_heap[i].id != id) {
            continue;
        }

        task_heap[i].active = 0;

        return 0;
    }

    return 1;
}


void reactor_update() {
    // If we are sleeping this will wake up the reactor.
    sleep_wakeup();
}


void reactor_disable_sleep() {
    // Stop the sleeper from entering power save mode.
    sleep_power_save_disable();
}


void reactor_enable_sleep() {
    // Allow the sleeper from entering power save mode.
    sleep_power_save_enable();
}


void reactor() {
    while (1) {
        // Used to determine how soon the soonest task needs to be run. We will
        // only sleep until then.
        ms_t soonest = INT32_MAX;

        // Check each active task to determine whether they should be
        // triggered.
        for (int i = 0; i < TASK_HEAP_SIZE; i ++) {
            if (! task_heap[i].active) {
                continue;
            }

            if (task_heap[i].ms <= 0) {
                task_heap[i].active = 0;
                task_heap[i].task(task_heap[i].real_ms);
                continue;
            }

        }

        // Check each task that is still active and how long until they should be triggered.
        for (int i = 0; i < TASK_HEAP_SIZE; i ++) {
            if (! task_heap[i].active) {
                continue;
            }

            if (task_heap[i].ms < soonest) {
                soonest = task_heap[i].ms;
            }
        }

        // Just sleep until the next task requires our attention.
        ms_t real_ms = sleep_ms(soonest);

        // Update all of the active tasks with the amount of time that has
        // passed.
        for (int i = 0; i < TASK_HEAP_SIZE; i ++) {
            if (! task_heap[i].active) {
                continue;
            }

            task_heap[i].ms -= real_ms;
            task_heap[i].real_ms += real_ms;
        }
    }
}
