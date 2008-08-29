#include <config.h>
#include <queue.h>
#include <usart.h>
#include <serv_usart.h>

// QUEUE for communicating with GPIO server
t_queue queue4usart, *usart_dest_queue;

unsigned int received;
char received_char;
int i;
char *print_str;

/* ---------------------------------------------------------------------------
 *  Server task of GPIO module
 *  Parameter: none
 *  Returns: none
 *  ----------------------------------------------------------------------- */

void serv_usart() {
    // Variable to store the received message
    t_message msg;

    // Wait for the next massage...
    if (queue_get_non_blocking(&queue4usart, &msg) == QUEUE_OK) {
        switch (msg.command) {
            case USART_INIT:
                // If the message was a ZERO value, the USART should be 
                // initialized
                // usart_init(9600);
                usart_init(USART_BAUDRATE);
                break;
            case USART_SEND:
                // Sending character through USART
                usart_send_char((char)msg.param);
                break;
            case USART_RECV:
                // Setting output (destination) queue, 
                // where the recv. character is to be sent to
                usart_dest_queue = (t_queue *)msg.param;
                break;
            case USART_PRINT:
                // Sending string through USART
                print_str = (char *)msg.param;

                for (i = 0; print_str[i]; i++)
                    usart_send_char(print_str[i]);

                i = 0;
                break;
       }
    }

    if (usart_rx_ready()) {
        received_char = usart_recv_char();
        received = 1;
    }

    if (received && (usart_dest_queue != 0)) {
        msg.command = USART_RECVD;
        msg.param = received_char;

        if (queue_put_non_blocking(usart_dest_queue, &msg) == QUEUE_OK)
            received = 0;
    }
}
