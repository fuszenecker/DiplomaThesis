#include <config.h>
#include <queue.h>
#include <usart.h>
#include <serv_can.h>

// QUEUE for communicating with GPIO server
t_queue queue4can;

/* ---------------------------------------------------------------------------
 * Variables to store data to be sent and a flag
 * ------------------------------------------------------------------------ */

unsigned int can_data0, can_data1, can_addr, can_tbs;

/* ---------------------------------------------------------------------------
 *  Server task of CAN module
 *  Parameter: none
 *  Returns: none
 *  ----------------------------------------------------------------------- */

void serv_can() {
    // Variable to store the received message
    t_message msg;

    // Wait for the next massage...
    if (queue_get_non_blocking(&queue4can, &msg) == QUEUE_OK) {
        switch (msg.command) {
            case CAN_INIT:
                // If the message was a ZERO value, the GPIO should be 
                // initialized
                CAN_init(CLOCK_FREQ/CAN_BAUDRATE/32);
                CAN_set_filter(0, 0);

                // Clear data
                can_addr = can_data0 = can_data1 = 0;

                // No data to send
                can_tbs = 0;
                break;
            case CAN_DATA0:
                can_data0 = msg.param;
                break;
            case CAN_DATA1:
                can_data1 = msg.param;
                break;
            case CAN_ADDR:
                can_addr = msg.param;
                break;
            case CAN_SEND:
                // Sending CAN frame with default parameters
                can_tbs = 1;
                break;
        }
    }

    if (can_tbs /* & CAN_tx_ready() */)
        CAN_send(can_addr, 8, can_data0, can_data1);
        can_tbs = 0;
}
