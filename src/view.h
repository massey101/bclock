#ifndef VIEW_H
#define VIEW_H

#include "epdpaint.h"
#include "alarms.h"
#include "ds3231.h"

/**
 * Initialise the viewer which will draw information to the screen using the
 * epdpainter.
 */
void view_init();

/**
 * Draw the following information to the screen.
 *
 * datetime - The current time to show
 * alarms - An array of alarms to show
 */
void view_update(vdatetime_t * datetime, valarm_t * alarms);

#endif // VIEW_H
