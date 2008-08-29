/* ---------------------------------------------------------------------------
 * Header file of the function that initializes user applications.
 * ------------------------------------------------------------------------ */

#ifndef __APPS_INIT_H__
#define __APPS_INIT_H__

#include <sched.h>

/* ---------------------------------------------------------------------------
 * Initializes user applications
 * Parameters: pointer to the process table (to register tasks)
 * ------------------------------------------------------------------------ */

void apps_init(t_process_table);

#endif
