// ---------------------------------------------------------------------------
// This file contains the "main()" function of the microcontroller.
// ---------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------
// Global variables: queues to the CAN and USART routines.
// ---------------------------------------------------------------------------

volatile t_queue usart2can;
volatile t_queue can2usart;

// ---------------------------------------------------------------------------
// These variables contain the CAN baudrate and the filter/mask serrings.
// ---------------------------------------------------------------------------

volatile unsigned int can_baudrate;
extern const unsigned int f_cpu;                      // Frequency of CPU clock
volatile unsigned int can_filter[14], can_id[14];

// ---------------------------------------------------------------------------
// The "main()" function is started by the "sysinit()" routine.
// The "sysinit()" is the function that is started when the MCU starts.
// ---------------------------------------------------------------------------

int main() {
    // A string to store text message to the "upper" CPU or user
    char str[MAX_STR_LEN];

    // Pointer to a message in the message queue
    t_message *pmsg;

    // Predefine the value of the CAN baud rate 
    can_baudrate = CAN_BAUDRATE;

    // Until I realize nirvana...
    while (1) {
        // Is there a new message in the USART->CAN queue?
        // The message arrives from the USART ISR.
        if (queue_get_non_blocking((t_queue *) &usart2can, &pmsg) == QUEUE_OK) {
            switch (pmsg->command) {
                case CAN_VERSION:
                    // Get version information:
                    // A text message will be sent: 
                    // constant text + VERSION string 
                    // (which can be seen in "config.h")
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
                    // Filter that makes the CAN MAC accepts all messages
                    CAN_set_filter(0, 0, 0);
                    break;

                case CAN_SET_BAUD:
                    // Predefined baud tare shall be changed
                    can_baudrate = pmsg->param1;
                    // See equation above
                    CAN_set_prescaler(f_cpu / 32 / can_baudrate - 1);

                    // Human-readable string...
                    usart_send_str("+ Baud rate: 0x");
                    // + the numeric of baud rate converted to hexadecimal format
                    num2hex(can_baudrate, str, 8);
                    // Send hexadecinal "string"
                    usart_send_str(str);

                    usart_send_str(", divisor: 0x");
                    num2hex(f_cpu / 32 / can_baudrate - 1, str, 8);
                    usart_send_str(str);
                    break;

                case CAN_SET_FILTER:
                    // Set filter mask value:
                    // The filter identifier must be less than 14!
                    if ((pmsg->param1 < 14) && (pmsg->param1 >= 0)) {
                        // Save mask value to the array defined above
                        can_filter[pmsg->param1] = pmsg->param2 & 0x1fffffff;
                        // Set filter according to the saved value
                        CAN_set_filter(pmsg->param1, can_filter[pmsg->param1],
                            can_id[pmsg->param1]);

                        // A little string to the user:
                        // The values will be sent back
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
                        // if the filter ID >= 14 or ID < 0, then an
                        // error message should be sent
                        usart_send_str("- Invalid filter number.");
                    }

                    break;

                case CAN_SET_ID:
                    // Set ID value:
                    // The filter identifier must be less than 14!
                    if ((pmsg->param1 < 14) && (pmsg->param1 >= 0)) {
                        // Save filter ID value to the array defined above
                        can_id[pmsg->param1] = pmsg->param2 & 0x1fffffff;
                        // Set filter according to the saved value
                        CAN_set_filter(0, can_filter[pmsg->param1], can_id[pmsg->param1]);

                        // A little string to the user:
                        // The values will be sent back
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
                        // if the filter ID >= 14 or ID < 0, then an
                        // error message should be sent
                        usart_send_str("- Invalid filter number.");
                    }

                    break;

                case CAN_SEND:
                    // Send message through CAN bus:
                    // Try to send the message, on success...
                    if (CAN_send(pmsg->param1 & 0x1fffffff, 
                                 (pmsg->param1 >> 29) + 1, 
                                 pmsg->param2, pmsg->param3) == CANTXOK) {
                        // ... a positive acknowledge is sent ...
                        usart_send_str("+ Message successfully sent.");
                    } else {
                        // ... else a warning will be received on USART
                        usart_send_str("- Unable to send message.");
                    }
                    break;

                case CAN_UNKNOWN:
                    // When an unknown CAN message arrives, a warning should appear
                    usart_send_str("- Unknown message arrived.");
                    break;

                default:
                    // This is the message that will not be seen (hopefully).
                    usart_send_str("- Internal error! Stop using this device!");
                    break;
            }

            // After sending the message(s), a newline should be sent, as well.
            // But what kind of newline? CR+LF or LF only? 
            // It depends on the "upper" device or on the user.
            if (cr_needed)
                usart_send_str("\r\n");
            else
                usart_send_str("\n");

            // The received and processed message should be removed from
            // the queue, so that some space appaers in the queue.
            queue_remove((t_queue *) &usart2can);
        }

        // If there is a message in the CAN queue arrived from the CAN ISR...
        if (queue_get_non_blocking((t_queue *)&can2usart, &pmsg) == QUEUE_OK) {
            // Provide a string that contains the ID, ...
            usart_send_str("# id: 0x");
            num2hex(pmsg->param1 & 0x1fffffff, str, 8);
            usart_send_str(str);

            // ... length ...
            usart_send_str(", length: ");
            num2hex((pmsg->param1 >> 29) + 1, str, 8);
            usart_send_str(str);

            // ... and data field.
            usart_send_str(", data: 0x");
            num2hex(pmsg->param2, str, 8);
            usart_send_str(str);
            num2hex(pmsg->param3, str, 8);
            usart_send_str(str);

            // The string will be terminated as described above.
            if (cr_needed)
                usart_send_str("\r\n");
            else
                usart_send_str("\n");

            // The received and processed message should be removed from
            // the queue, so that some space appaers in the queue.
            queue_remove((t_queue *) &can2usart);
        }
    } /* while (1) ... */
}

