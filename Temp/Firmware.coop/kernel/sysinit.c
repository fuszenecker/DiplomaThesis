/* ---------------------------------------------------------------------------
 * This file contains the startup code for the ARM-Cortex microcontroller.
 * ------------------------------------------------------------------------ */

#include <config.h>
#include <sysinit.h>
#include <clock.h>
#include <gpio.h>
#include <systick.h>
#include <irq.h>

#include <sched.h>
#include <queue.h>
#include <servers_init.h>
#include <apps_init.h>

#include <serv_gpio.h>
#include <serv_usart.h>
#include <serv_can.h>

/* ---------------------------------------------------------------------------
 * The first word of the FLASH should be the initial stack pointer of the
 * microcontroller.
 * This parameter will be in the ".stack_pointer" section.
 * See also: linker script
 * ------------------------------------------------------------------------ */

__attribute__((section(".stack_pointer")))
void *stack_pointer = (void *) (RAM_BASE + MAIN_STACK);

/* ---------------------------------------------------------------------------
 * The next words should be the pointers to ISRs (interrupt service routines).
 * This parameter will be in the ".vectors" section.
 * See also: linker script
 * ------------------------------------------------------------------------ */

__attribute__((section(".vectors")))
void (*vectors[])() = { sysinit, 0, 0, 0, 0,
                        0, 0, 0, 0, 0,
                        0, 0, 0, 0, systick
};

/* Process descriptor table, that contains the data about processes */
t_process_descriptor process_table[PROC_TAB_SIZE];

/* ---------------------------------------------------------------------------
 * The function will be started after RESET.
 * ------------------------------------------------------------------------ */

void sysinit() {
    unsigned int *ptr;

    for (ptr = (unsigned int *)0x20000000; ptr < (unsigned int *)(0x20000000 + 1 K); ptr++)
        *ptr = 0;

    /* Index of the currently running task */
    t_pid current_process;

    /* A small message */
    t_message msg;

#if CLOCK_QUARTZ == 1
    /* If the CLOCK_QUARTZ is 1, the main oscillator will be enabled */
    clock_enable_main_osc();
#if CLOCK_PLL
    /* If the CLOCK_PLL is 1, the PLL will be enabled (requires main oscillator) */
    clock_enable_pll();
#endif
#endif

    /* Low level initialization of the GPIO ports */
    // gpio_init();
    // systick_init(1000);

    /* Initialization of the process scheduler */
    sched_init(process_table);

    /* Adding server task into process table */
    servers_init(process_table);

    /* Adding applicarion task into process table */
    apps_init(process_table);

    /* Initialization of the server processes */
    /* by sending a zero value to the server task */
    msg.command = msg.param = GPIO_INIT;
    queue_put_non_blocking(&queue4gpio, &msg);
    msg.command = msg.param = USART_INIT;
    queue_put_non_blocking(&queue4usart, &msg);
    msg.command = msg.param = CAN_INIT;
    queue_put_non_blocking(&queue4can, &msg);

    /* Main loop */
    while (1)
        /* Choose the next task and run it */
        sched_execute(process_table, &current_process);
}

