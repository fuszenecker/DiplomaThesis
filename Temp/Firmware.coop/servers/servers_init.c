/* ---------------------------------------------------------------------------
 * File of the function that initializes the server applications.
 * ------------------------------------------------------------------------ */

#include <servers_init.h>
#include <serv_gpio.h>
#include <serv_usart.h>
#include <serv_can.h>

/* ---------------------------------------------------------------------------
 * Initializes the server applications
 * Parameters: pointer to the process table (to register tasks)
 * ------------------------------------------------------------------------ */

void servers_init(t_process_table process_table) {
    // Initializing the queue for communicating with the GPIO server
    queue_init(&queue4gpio);

    // Adding GPIO server to the process list
    sched_add(process_table, serv_gpio, "GPIO Server     ", 1);

    // Initializing the queue for communicating with the USART server
    queue_init(&queue4usart);

    // Adding USART server to the process list
    sched_add(process_table, serv_usart, "USART Server    ", 5);

    // Initializing the queue for communicating with the CAN server
    queue_init(&queue4can);

    // Adding CAN server to the process list
    sched_add(process_table, serv_can, "CAN Server      ", 1);
}
