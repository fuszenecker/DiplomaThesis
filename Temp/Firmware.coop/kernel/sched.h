/* ---------------------------------------------------------------------------
 * This file contains the function that are used to manage processes
 * ------------------------------------------------------------------------ */

#ifndef __SCHED_H__
#define __SCHED_H__

/* ---------------------------------------------------------------------------
 * If the length of the process table is not defined, the default value
 * will be 16 (items).
 * ------------------------------------------------------------------------ */

#ifndef PROC_TAB_SIZE
#define	PROC_TAB_SIZE	16
#endif

#include <config.h>

/* A process (item) can be: not used, not running (stopped) or running */
typedef enum {SCHED_FREE, SCHED_NOT_RUNNING, SCHED_RUNNING} t_sched_status;

/* Return value of the scheduler functions */
typedef enum {SCHED_OK, SCHED_ERROR} t_sched_result;

typedef unsigned int t_priority;
typedef unsigned int t_pid;

/* ---------------------------------------------------------------------------
 * The process list contains:
 *      - the status: can be: free, not running or running
 *      - name of the task
 *      - priority of the task 
 *          (0: highest proitity, positiv values: lower priorities)
 *      - priority preload: real priority vale
 *      - pointer to the "main()" function of the task
 * ------------------------------------------------------------------------ */

typedef struct {
    t_sched_status status;
    char name[TASK_NAME_LEN+1];
    t_priority priority;
    t_priority priority_preload;
    void (*function)();
} t_process_descriptor;

typedef t_process_descriptor* t_process_table;

/* ---------------------------------------------------------------------------
 * Initializes the scheduler.
 * Parameter: pointer of the process table
 * ------------------------------------------------------------------------ */

void sched_init(t_process_table);

/* ---------------------------------------------------------------------------
 * Adds a new task into the process list.
 * Parameters:
 *      - pointer to the process table
 *      - address of the function to be run periodically
 *      - name of the task
 *      - priority of the process
 * Returns:
 *      - SCHED_OK if it was successful
 *      - SCHED_ERROR if there was an error
 * ------------------------------------------------------------------------ */

t_sched_result sched_add(t_process_table, void (*)(), char *, t_priority);

/* ---------------------------------------------------------------------------
 * Deletes a task from the process list.
 * Parameters:
 *      - pointer to the process table
 *      - index of the task
 * Returns:
 *      - SCHED_OK if it was successful
 *      - SCHED_ERROR if there was an error
 * ------------------------------------------------------------------------ */

t_sched_result sched_delete(t_process_table, t_pid);

/* ---------------------------------------------------------------------------
 * Sets the task status.
 * Parameters:
 *      - pointer to the process table
 *      - index of the task
 *      - new status value
 * Returns:
 *      - SCHED_OK if it was successful
 *      - SCHED_ERROR if there was an error
 * ------------------------------------------------------------------------ */

t_sched_result sched_set_status(t_process_table, t_pid, t_sched_status);

/* ---------------------------------------------------------------------------
 * Sets the task priority.
 * Parameters:
 *      - pointer to the process table
 *      - index of the task
 *      - new priority value
 * Returns:
 *      - SCHED_OK if it was successful
 *      - SCHED_ERROR if there was an error
 * ------------------------------------------------------------------------ */

t_sched_result sched_set_priority(t_process_table, t_pid, t_priority);

/* ---------------------------------------------------------------------------
 * Executes the next runnable task accordingly to the priority values
 * Parameters:
 *      - pointer to the process table
 * Returns:
 *      - SCHED_OK if it was successful
 *      - SCHED_ERROR if there was an error
 * ------------------------------------------------------------------------ */

void sched_execute(t_process_table, t_pid *);

#endif
