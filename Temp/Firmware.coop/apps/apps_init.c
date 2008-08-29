/* ---------------------------------------------------------------------------
 * This file contains the function that initializes the user applications.
 * ------------------------------------------------------------------------ */

#include <apps_init.h>
#include <gpio.h>
#include <sched.h>
#include <serv_gpio.h>

#include <led1.h>
#include <led2.h>
#include <taskman.h>

extern unsigned int cank;

/* ---------------------------------------------------------------------------
 * This function initializes the user applications.
 * Parameters:
 *      * pointer to the process table
 * ------------------------------------------------------------------------ */

void apps_init(t_process_table process_table) {
    /* Add task to the process table with name "..." and priority ... */
    sched_add(process_table, task1, "Task 1          ", 11);
    cank = 0;

    /* Add task to the process table with name "..." and priority ... */
    sched_add(process_table, task2, "Task 2          ", 10);

    // Initializing the queue for communicating with the CAN server
    queue_init(&queue4taskman);

    /* Add task to the process table with name "..." and priority ... */
    sched_add(process_table, taskman, "Task Manager    ", 100);
    taskman_state = 0;
}

