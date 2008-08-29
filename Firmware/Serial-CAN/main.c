/* ---------------------------------------------------------------------------
 * This file contains the startup code for the ARM-Cortex microcontroller.
 * ------------------------------------------------------------------------ */

#include <config.h>
#include <sysinit.h>
#include <queue.h>
#include <num_conv.h>

#include <irq.h>
#include <gpio.h>
#include <usart.h>
#include <clock.h>
#include <can.h>
#include <can_cmd.h>

volatile t_queue usart2can;
volatile t_queue can2usart;
volatile unsigned int can_baudrate;
volatile unsigned int can_filter[14], can_id[14];
extern const unsigned int f_cpu;

int main() {
    char str[MAX_STR_LEN];

    /* A small message */
    t_message *pmsg;

    can_baudrate = CAN_BAUDRATE;

    while (1) {
        if (queue_get_non_blocking((t_queue *) &usart2can, &pmsg) == QUEUE_OK) {
            switch (pmsg->command) {
                case CAN_VERSION:
                    usart_send_str("+ SER_CAN: ");
                    usart_send_str(DEV_VERSION);
                    break;

                case CAN_INIT:
                    // bitrate = 1/(tq * 16)
                    // tq = 1/(f_cpu / 2) * (BRD+1) = 2/f_cpu * (BRD+1)
                    // bitrate = 1 / [2/f_cpu * 16 * (BRD+1)] = 
                    //     = f_cpu / [32 * (BRD+1)]
                    // BRD+1 = f_cpu / 32 / bitrate
                    // BRD = f_cpu / 32 / bitrate - 1
                    CAN_set_prescaler(f_cpu / 32 / can_baudrate - 1);
                    usart_send_str("+ Successfully initialized.");
                    CAN_set_filter(0, 0, 0);
                    CAN_set_filter(1, 0x1fffffff, 0);
                    break;

                case CAN_SET_BAUD:
                    can_baudrate = pmsg->param1;
                    CAN_set_prescaler(f_cpu / 32 / can_baudrate - 1);

                    usart_send_str("+ Baud rate: 0x");
                    num2hex(can_baudrate, str, 8);
                    usart_send_str(str);

                    usart_send_str(", divisor: 0x");
                    num2hex(f_cpu / 32 / can_baudrate - 1, str, 8);
                    usart_send_str(str);
                    break;

                case CAN_SET_FILTER:
                    if ((pmsg->param1 < 14) && (pmsg->param1 >= 0)) {
                        can_filter[pmsg->param1] = pmsg->param2 & 0x1fffffff;
                        CAN_set_filter(pmsg->param1, can_filter[pmsg->param1],
                            can_id[pmsg->param1]);

                        usart_send_str("+ Filter 0x");
                        num2hex(pmsg->param1, str, 1);
                        usart_send_str(str);

                        usart_send_str(": Mask: 0x");
                        num2hex(can_filter[pmsg->param1], str, 8);
                        usart_send_str(str);

                        usart_send_str(", Id: 0x");
                        num2hex(can_id[pmsg->param1], str, 8);
                        usart_send_str(str);
                    } else {
                        usart_send_str("- Invalid filter number.");
                    }

                    break;

                case CAN_SET_ID:
                    if ((pmsg->param1 < 14) && (pmsg->param1 >= 0)) {
                        can_id[pmsg->param1] = pmsg->param2 & 0x1fffffff;
                        CAN_set_filter(0, can_filter[pmsg->param1], can_id[pmsg->param1]);

                        usart_send_str("+ Filter 0x");
                        num2hex(pmsg->param1, str, 1);
                        usart_send_str(str);

                        usart_send_str(": Mask: 0x");
                        num2hex(can_filter[pmsg->param1], str, 8);
                        usart_send_str(str);

                        usart_send_str(", Id: 0x");
                        num2hex(can_id[pmsg->param1], str, 8);
                        usart_send_str(str);
                    } else {
                        usart_send_str("- Invalid filter number.");
                    }

                    break;

                case CAN_SEND:
                    if (CAN_send(pmsg->param1 & 0x1fffffff, 
                                 (pmsg->param1 >> 29) + 1, 
                                 pmsg->param2, pmsg->param3) == CANTXOK) {
                        usart_send_str("+ Message successfully sent.");
                    } else {
                        usart_send_str("- Unable to send message.");
                    }
                    break;

                case CAN_UNKNOWN:
                    usart_send_str("- Unknown message arrived.");
                    break;

                default:
                    usart_send_str("- Internal error! Stop using this device!");
                    break;
            }

            if (cr_needed)
                usart_send_str("\r\n");
            else
                usart_send_str("\n");

            queue_remove((t_queue *) &usart2can);
        }

        if (queue_get_non_blocking((t_queue *) &can2usart, &pmsg) == QUEUE_OK) {
            usart_send_str("# id: 0x");
            num2hex(pmsg->param1 & 0x1fffffff, str, 8);
            usart_send_str(str);

            usart_send_str(", length: ");
            num2hex((pmsg->param1 >> 29) + 1, str, 8);
            usart_send_str(str);

            usart_send_str(", data: 0x");
            num2hex(pmsg->param2, str, 8);
            usart_send_str(str);
            num2hex(pmsg->param3, str, 8);
            usart_send_str(str);

            if (cr_needed)
                usart_send_str("\r\n");
            else
                usart_send_str("\n");

            queue_remove((t_queue *) &can2usart);
        }
    } /* while (1) ... */
}

