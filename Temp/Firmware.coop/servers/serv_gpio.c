#include <config.h>
#include <queue.h>
#include <gpio.h>
#include <serv_gpio.h>

// QUEUE for communicating with GPIO server
t_queue queue4gpio;

/* ---------------------------------------------------------------------------
 *  Server task of GPIO module
 *  Parameter: none
 *  Returns: none
 *  ----------------------------------------------------------------------- */

void serv_gpio() {
    // Variable to store the received message
    t_message msg;

    // Wait for the next massage...
    if (queue_get_non_blocking(&queue4gpio, &msg) == QUEUE_OK)
        switch (msg.command) {
            case GPIO_INIT:
                // If the message was a ZERO value, the GPIO should be 
                // initialized
                gpio_init();
                break;
            case GPIO_SET:          
                // Sets appropriate GPIO bits
                gpio_set(msg.param);
                break;
            case GPIO_CLEAR:        
                // Clears appropriate GPIO bits
                gpio_clear(msg.param);
                break;
            default:
                break;
        }
}
