/* ---------------------------------------------------------------------------
 * Header file of the function that initializes the server applications.
 * ------------------------------------------------------------------------ */

#ifndef __SERVERS_INIT_H__
#define __SERVERS_INIT_H__

#include <sched.h>

/* ---------------------------------------------------------------------------
 * Initializes the server applications
 * Parameters: pointer to the process table (to register tasks)
 * ------------------------------------------------------------------------ */

void servers_init(t_process_table);

#endif
