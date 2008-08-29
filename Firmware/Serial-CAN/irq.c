// ---------------------------------------------------------------------------
// This file contains the Interrupt Service Routines (ISR) 
// used by the firmware.
// ---------------------------------------------------------------------------

// void systick(void) __attribute__((__interrupt__));
// void usart_rx_handler(void) __attribute__((__interrupt__));

#include <config.h>
#include <gpio.h>
#include <usart.h>
#include <stm32f10x_can.h>
#include <can_cmd.h>

#include <queue.h>
#include <num_conv.h>

// ---------------------------------------------------------------------------
// There are two "diagnostic" LEDs used by the program.
// The first one is to indicate that the state machine started to receive
// a new command through serial line.
// The second LED is a blinking one, which shows that the device is working.
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// PORT B 15 (blibking) and 14 (receiving new message).
// ---------------------------------------------------------------------------

#define LED_SYSTICK (1 << 15)
#define LED_USART   (1 << 14)

// ---------------------------------------------------------------------------
// Global and volatile variables: line received from the upper controller.
// ---------------------------------------------------------------------------

volatile char line[MAX_STR_LEN];
volatile unsigned int line_len;
volatile unsigned int cr_needed;

// ---------------------------------------------------------------------------
// Message queues to CAN and to USART controlling routines.
// ---------------------------------------------------------------------------

extern t_queue usart2can;
extern t_queue can2usart;

// ---------------------------------------------------------------------------
// Stores the value of the "blibking" LED.
// ---------------------------------------------------------------------------

volatile unsigned int counter;

// ---------------------------------------------------------------------------
// ISR of the SYSTICK timer (makes the LED blibking).
// ---------------------------------------------------------------------------

void systick() {
    counter++;

    if (counter % 2 == 1) {
        gpio_set(LED_SYSTICK);
    } else {
        gpio_clear(LED_SYSTICK);
    }
}

// ---------------------------------------------------------------------------
// When a character has been received through USART...
// ---------------------------------------------------------------------------

void usart_rx_handler() {
    char ch;
    t_message msg;

    // If a new character is received
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        // Switch on "Receiging..." LED
        gpio_set(LED_USART);

        // Read the character from the USART FIFO
        ch = usart_recv_char();

        // A CR (Cursor Return) indicates that CR must be sent after
        // the answer string.

        if (ch == '\r') {
            cr_needed = 1;
        }

        // At the end of the command line...
        if ((ch == '\n') || (ch == '\r')) {
            // ... when the command starts with...
            switch (line[0]) {
                case 'v':
                case 'V':
                    // Version info
                    // One character expected
                    msg.command = CAN_VERSION;
                    msg.param1 = msg.param2 = msg.param3 = 0;
                    break;

/*              
                ---- REMOVED FROM THE STABLE RELEASE -------------------------
                case 'r':
                case 'R':
                    // Initialize CAN
                    // One character expected
                    msg.command = CAN_INIT;
                    msg.param1 = msg.param2 = msg.param3 = 0;
                    break;
                --------------------------------------------------------------
*/
                case 'b':
                case 'B':
                    // Set baud rate
                    // Format: B_xxxxxxxx
                    // where 'x' is a hexadecimal digit
                    // and '_' is "don't care"
                    msg.command = CAN_SET_BAUD;
                    msg.param1 = hex2num((char *) line+2, 8);
                    msg.param2 = msg.param3 = 0;
                    break;

                case 'f':
                case 'F':
                    // Set filter mask
                    // Format: Fnxxxxxxxx
                    // where 'xxxxxxxx' is the mask in hexadecimal format
                    // 'n' is the filter's number (0 ... 13) in hexadecimal
                    msg.command = CAN_SET_FILTER;
                    msg.param1 = hex2num((char *) line+1, 1);
                    msg.param2 = hex2num((char *) line+2, 8);
                    msg.param3 = 0;
                    break;

                case 'i':
                case 'I':
                    // Set CAN id
                    // Format: Inxxxxxxxx
                    // where 'xxxxxxxx' is the ID value in hexadecimal
                    // 'n' is the filter's number (0 ... 13) in hexadecimal
                    msg.command = CAN_SET_ID;
                    msg.param1 = hex2num((char *) line+1, 1);
                    msg.param2 = hex2num((char *) line+2, 8);
                    msg.param3 = 0;
                    break;

                case 's':
                case 'S':
                    // Send CAN frame
                    // Format: Sniiiiiiiidddddddddddddddd
                    // where 'n' is the number of bytes to be sent (1 ... 8)
                    //      it is not possible to send 0 byte of data!
                    // "iiiiiiii" is the destination id in HEXADECIMAL
                    // "dd ... dd" is the data. 16 digits (8 bytes) expected.
                    msg.command = CAN_SEND;

                    if (line[1] == '0')
                        line[1] = '1';

                    msg.param1 = (hex2num((char *) line+1, 1) - 1) << 29 | 
                                  (hex2num((char *) line+2, 8) & 0x1fffffff);
                    msg.param2 = hex2num((char *) line+10, 8);
                    msg.param3 = hex2num((char *) line+18, 8);
                    break;

                default:
                    // When received an invalid command
                    msg.command = CAN_UNKNOWN;
                    msg.param1 = msg.param2 = msg.param3 = 0;
                    break;
            }
        
            // Sending the CAN command to the routine that
            // utilizes CAN controller
            queue_put_non_blocking(&usart2can, &msg);
            // Switch off the "Receiving..." LED
            // at the end of the command line
            gpio_clear(LED_USART);
            // Line is to be invalidated
            line_len = 0;
            return;
        } else {
            // If one more character can be attached to the string 
            // (command line)...
            if (line_len + 1 < MAX_STR_LEN) {
                // ... attach the character, and the terminator "zero"
                line[line_len++] = ch;
                line[line_len] = 0;
            }
        }
    }
}

// ---------------------------------------------------------------------------
// When a message (frame) has been received through CAN controller...
// ---------------------------------------------------------------------------

void CAN_rx_handler() {
    t_message msg;
    CanRxMsg RxMessage;

    // Clean invalid data from the structure
    RxMessage.StdId = 0x00;
    RxMessage.ExtId = 0x00;
    RxMessage.IDE = 0;
    RxMessage.DLC = 0;
    RxMessage.FMI = 0;
    RxMessage.Data[0] = 0x00;
    RxMessage.Data[1] = 0x00;
    RxMessage.Data[2] = 0x00;
    RxMessage.Data[3] = 0x00;
    RxMessage.Data[4] = 0x00;
    RxMessage.Data[5] = 0x00;
    RxMessage.Data[6] = 0x00;
    RxMessage.Data[7] = 0x00;

    // Copy data from CAN FIFO to structure
    CAN_Receive(CAN_FIFO0, &RxMessage);

    // If it is in the acceptable format (with 29 bit addresses)...
    if (RxMessage.IDE == CAN_ID_EXT) {
        // At least one byte of data has to be received (and sent!)
        if (RxMessage.DLC == 0) {
            RxMessage.DLC = 1;
        }

        // Preparing the massage to be sent to "main()" function
        msg.command = 0;
        msg.param1 = RxMessage.ExtId | (RxMessage.DLC - 1) << 29;
        msg.param2 = (RxMessage.Data[0] << 24) | (RxMessage.Data[1] << 16) |
                     (RxMessage.Data[2] << 8) | RxMessage.Data[3];
        msg.param3 = (RxMessage.Data[4] << 24) | (RxMessage.Data[5] << 16) |
                     (RxMessage.Data[6] << 8) | RxMessage.Data[7];

        // Send message to "main()", which will send it to "upper"
        // controller
        queue_put_non_blocking(&can2usart, &msg);
    }
}
