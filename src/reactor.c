#include <string.h>
#include <limits.h>
#include "reactor.h"

struct task {
    uint8_t active;
    ms_t ms;
    ms_t real_ms;
};


// Each task is given a location in the array in which it is stored.
static volatile struct task active_tasks[NUM_TASKS] = {{0}};


void reactor_call_later(enum task_id id, ms_t ms) {
    active_tasks[id].active = 1;
    active_tasks[id].ms = ms;
    active_tasks[id].real_ms = 0;
}


void reactor_cancel(enum task_id id) {
    active_tasks[id].active = 0;
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
        for (int i = 0; i < NUM_TASKS; i ++) {
            if (! active_tasks[i].active) {
                continue;
            }

            if (active_tasks[i].ms <= 0) {
                active_tasks[i].active = 0;
                tasks[i](active_tasks[i].real_ms);
                continue;
            }

        }

        // Check each task that is still active and how long until they should be triggered.
        for (int i = 0; i < NUM_TASKS; i ++) {
            if (! active_tasks[i].active) {
                continue;
            }

            if (active_tasks[i].ms < soonest) {
                soonest = active_tasks[i].ms;
            }
        }

        // Just sleep until the next task requires our attention.
        ms_t real_ms = sleep_ms(soonest);

        // Update all of the active tasks with the amount of time that has
        // passed.
        for (int i = 0; i < NUM_TASKS; i ++) {
            if (! active_tasks[i].active) {
                continue;
            }

            active_tasks[i].ms -= real_ms;
            active_tasks[i].real_ms += real_ms;
        }
    }
}
