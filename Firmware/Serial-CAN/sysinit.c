/* ---------------------------------------------------------------------------
 * This file contains the startup code for the ARM-Cortex microcontroller.
 * ------------------------------------------------------------------------ */

#include <config.h>
#include <queue.h>
#include <main.h>
#include <sysinit.h>

#include <clock.h>
#include <gpio.h>
#include <usart.h>
#include <systick.h>
#include <irq.h>
#include <can.h>
#include <can_cmd.h>

/* ---------------------------------------------------------------------------
 * The first word of the FLASH should be the initial stack pointer of the
 * microcontroller.
 * This parameter will be in the ".stack_pointer" section.
 * See also: linker script
 * ------------------------------------------------------------------------ */

__attribute__((section(".stack_pointer")))
void *stack_pointer = (void *) (MAIN_STACK);

/* ---------------------------------------------------------------------------
 * The next words should be pointers to ISRs (Interrupt Service Routines).
 * These parameters will be placed into the ".vectors" section.
 * See also: linker script
 * ------------------------------------------------------------------------ */

void no_handler();

__attribute__((section(".vectors")))
void (*vectors[])() = { 
    sysinit, no_handler, no_handler, no_handler, no_handler,
    no_handler, no_handler, no_handler, no_handler, no_handler,
    no_handler, no_handler, no_handler, no_handler, systick,
    no_handler, no_handler, no_handler, no_handler, no_handler,
    no_handler, no_handler, no_handler, no_handler, no_handler,
    no_handler, no_handler, no_handler, no_handler, no_handler,
    no_handler, no_handler, no_handler, no_handler, no_handler,
    CAN_rx_handler, no_handler, no_handler, no_handler, no_handler,
    no_handler, no_handler, no_handler, no_handler, no_handler,
    no_handler, no_handler, no_handler, no_handler, no_handler,
    no_handler, no_handler, usart_rx_handler, no_handler, no_handler,
};

/* ---------------------------------------------------------------------------
 * The function will be started after RESET.
 * ------------------------------------------------------------------------ */

void no_handler() {
    for(;;);
}

t_queue usart2can;
t_queue can2usart;
const unsigned int f_cpu = CLOCK_FREQ * PLL_FACTOR;

void sysinit() {
//    unsigned char *ptr;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* If the CLOCK_QUARTZ is 1, the main oscillator will be enabled */
    clock_enable_main_osc();
    /* If the CLOCK_PLL is 1, the PLL will be enabled (requires main oscillator) */
    clock_enable_pll();

    /* Low level initialization of the GPIO ports */
    gpio_init();
    usart_init(USART_BAUDRATE);
    CAN_init(f_cpu / 32 / CAN_BAUDRATE - 1);
    CAN_set_filter(0, 0, 0);

    cr_needed = line_len = 0;
    queue_init(&usart2can);
    queue_init(&can2usart);

    /* Initialization of the Systick Timer */
    systick_init(4);

    /* Set the Vector Table base location at 0x08000000 */
    // NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);

    /* Configure the NVIC Preemption Priority Bits */
    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    /* Enable the USART1 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
    // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    /* Enable CAN RX0 interrupt IRQ channel */
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN_RX0_IRQChannel;
    // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* CAN FIFO0 and FIFO1 message pending interrupt enable */
    CAN_ITConfig(CAN_IT_FMP0, ENABLE);
    CAN_ITConfig(CAN_IT_FMP1, ENABLE);

    while (1)
        main();
}

