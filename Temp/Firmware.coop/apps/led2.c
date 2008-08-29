/* ---------------------------------------------------------------------------
 * This file contains the main() function of a user applications.
 * ------------------------------------------------------------------------ */

#include <apps_init.h>
#include <gpio.h>
#include <sched.h>
#include <serv_gpio.h>
#include <serv_usart.h>

#include <led2.h>

void task2() {
    static unsigned int c;
    t_message msg;

    if (((c) % D) == D/2) {
        msg.command = GPIO_SET;
        msg.param = (1 << 14);
        queue_put_non_blocking(&queue4gpio, &msg);
/*
        msg.command = 1;
        msg.param = '*';
        queue_put_non_blocking(&queue4usart, &msg);
*/
    }

    if (((++c) % D) == 0) {
        msg.command = GPIO_CLEAR;
        msg.param = (1 << 14);
        queue_put_non_blocking(&queue4gpio, &msg);
    }
}

