/* ---------------------------------------------------------------------------
 * This is a comfiguration file (header file) of the oprtating system.
 * ------------------------------------------------------------------------ */

#ifndef __CONFIG_H__
#define __CONFIG_H__

/* The size of the main stack located at the end of the RAM area */
#define	K		    *1024   /* Definition of "1 kilobyte" */
#define STACK_SIZE	(1 K)
#define MAX_STR_LEN 64      /* Maximum length of the strings */

/* ---------------------------------------------------------------------------
 * Clock selection
 * ------------------------------------------------------------------------ */

/* Shall the QUARTZ oscillator be enabled? */
#define CLOCK_QUARTZ    1

/* Shall the PLL be enablesd */
#define CLOCK_PLL       1
#define CLOCK_FREQ      12000000
#define PLL_DIV         RCC_PLLSource_HSE_Div1
#define PLL_MUL         RCC_PLLMul_6

/* ---------------------------------------------------------------------------
 * Stack definitions: the origin and the size
 * ------------------------------------------------------------------------ */

#ifdef __STM32__
#define RAM_BASE        0x20000000
#define RAM_SIZE        (8 K)
#endif

#define MAIN_STACK      RAM_SIZE
// #define PROC_STACK(n)   (RAM_SIZE - (n) * STACK_SIZE)

/* ---------------------------------------------------------------------------
 * Few constants of the kernel core
 * ------------------------------------------------------------------------ */

#define PROC_TAB_SIZE   16      /* Number of processes */
#define QUEUE_SIZE      16      /* Entries in the message queues */
#define TASK_NAME_LEN   16      /* Maximum length of the process' name */

#define CAN_BAUDRATE    25000
#define USART_BAUDRATE  115200

#endif
