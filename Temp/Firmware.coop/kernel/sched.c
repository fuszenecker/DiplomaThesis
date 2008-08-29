/* ---------------------------------------------------------------------------
 * This file contains the function that are used to manage processes
 * ------------------------------------------------------------------------ */

#include <sched.h>
#include <can.h>

/* ---------------------------------------------------------------------------
 * Initializes the scheduler.
 * Parameter: pointer of the process table
 * ------------------------------------------------------------------------ */

void sched_init(t_process_table process_table) {
    int index;

    for (index = 0; index < PROC_TAB_SIZE; index++) {
        process_table[index].status = SCHED_FREE;
        process_table[index].priority = 0;
        process_table[index].name[0] = 0;
        process_table[index].priority_preload = 0;
        process_table[index].function = (void (*)()) 0;
    }
}

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

t_sched_result sched_add(t_process_table process_table, void (*function)(), char *name, t_priority priority) {
    int index, p;

    for (index = 0; index < PROC_TAB_SIZE; index++) {
        /* If the entry is free... */
        if (process_table[index].status == SCHED_FREE) {
            /* ... put values here */
            process_table[index].status = SCHED_RUNNING;

            /* Copy name into the process table */
            for (p = 0; name[p] && p < TASK_NAME_LEN; p++)
                process_table[index].name[p] = name[p];

            process_table[index].name[p] = 0;
            process_table[index].name[TASK_NAME_LEN] = 0;

            process_table[index].priority = priority;
            process_table[index].priority_preload = priority;
            process_table[index].function = function;

            return SCHED_OK;
        }
    }

    return SCHED_ERROR;
}

/* ---------------------------------------------------------------------------
 * Executes the next runnable task accordingly to the priority values
 * Parameters:
 *      - pointer to the process table
 * Returns:
 *      - SCHED_OK if it was successful
 *      - SCHED_ERROR if there was an error
 * ------------------------------------------------------------------------ */

void sched_execute(t_process_table process_table, t_pid *current_process) {
    do {
        /* Select next process */
        (*current_process)++;

        /* If the previous was the last one, we have to continue at */
        /* the first element */
        (*current_process) %= PROC_TAB_SIZE;
        
        /* Until the process can be run and the priority counter is zero */
    } while (!((process_table[*current_process].status == SCHED_RUNNING) && ((process_table[*current_process].priority--) == 0)));

    /* Priority counter has to be reset (preloaded) */
    process_table[*current_process].priority = process_table[*current_process].priority_preload;

    /* Call the selected function */
    process_table[*current_process].function();
    CAN_send(0x55aa00, 8, 0x55555555, 0xf0f0f0f0);
}

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

t_sched_result sched_set_status(t_process_table process_table, t_pid index, t_sched_status status) {
    if (index < PROC_TAB_SIZE) {
        process_table[index].status = status;
        return SCHED_OK;
    } else {
        return SCHED_ERROR;
    }
}

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

t_sched_result sched_set_priority(t_process_table process_table, t_pid index, t_priority priority) {
    if (index < PROC_TAB_SIZE) {
        process_table[index].priority_preload = priority;
        return SCHED_OK;
    } else {
        return SCHED_ERROR;
    }
}

/* ---------------------------------------------------------------------------
 * Deletes a task from the process list.
 * Parameters:
 *      - pointer to the process table
 *      - index of the task
 * Returns:
 *      - SCHED_OK if it was successful
 *      - SCHED_ERROR if there was an error
 * ------------------------------------------------------------------------ */

t_sched_result sched_delete(t_process_table process_table, t_pid index) {
    return sched_set_status(process_table, index, SCHED_FREE);
}
