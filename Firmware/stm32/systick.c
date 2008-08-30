#include <systick.h>
#include <stm32f10x_lib.h>
#include <stm32f10x_systick.h>
#include <config.h>

int systick_init(unsigned int freq) {
    SysTick_SetReload(CLOCK_FREQ * PLL_MUL / PLL_DIV / 8 / freq * 1024);
    SysTick_ITConfig(ENABLE);
    SysTick_CounterCmd(SysTick_Counter_Enable);
    
    return 0;
}