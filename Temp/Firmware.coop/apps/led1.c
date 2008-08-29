/* ---------------------------------------------------------------------------
 * This file contains the main() function of a user applications.
 * ------------------------------------------------------------------------ */

#include <apps_init.h>
#include <gpio.h>
#include <sched.h>
#include <serv_gpio.h>
#include <serv_usart.h>
#include <serv_can.h>

#include <led1.h>

unsigned int cank;

void task1() {
    static unsigned int c;
    t_message msg;

    if (((c) % D) == D/2) {
        msg.command = GPIO_SET;
        msg.param = (1 << 15);
        queue_put_non_blocking(&queue4gpio, &msg);

        msg.command = CAN_DATA0;
        msg.param = cank++;
        queue_put_non_blocking(&queue4can, &msg);

        msg.command = CAN_DATA1;
        msg.param = -(cank++);
        queue_put_non_blocking(&queue4can, &msg);

        msg.command = CAN_ADDR;
        msg.param = 0x55aaff;
        queue_put_non_blocking(&queue4can, &msg);

        msg.command = CAN_SEND;
        queue_put_non_blocking(&queue4can, &msg);
    }

    if (((++c) % D) == 0) {
        msg.command = GPIO_CLEAR;
        msg.param = (1 << 15);
        queue_put_non_blocking(&queue4gpio, &msg);

        msg.command = CAN_DATA0;
        msg.param = cank++;
        queue_put_non_blocking(&queue4can, &msg);

        msg.command = CAN_DATA1;
        msg.param = -(cank++);
        queue_put_non_blocking(&queue4can, &msg);

        msg.command = CAN_ADDR;
        msg.param = 0x55aaff;
        queue_put_non_blocking(&queue4can, &msg);

        msg.command = CAN_SEND;
        queue_put_non_blocking(&queue4can, &msg);
    }
}

