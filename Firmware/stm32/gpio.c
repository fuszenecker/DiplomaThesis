// ---------------------------------------------------------------------------
// This file contains the function definitions for General Purpose
// Input-Output ports.
// ---------------------------------------------------------------------------

#include <gpio.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>

// ---------------------------------------------------------------------------
// This function initializes the PORT B port.
// ---------------------------------------------------------------------------

void gpio_init() {
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

// ---------------------------------------------------------------------------
// Sets the bits of PORTB according to "data"
// PORTB |= data
// ---------------------------------------------------------------------------

void gpio_set(unsigned int data) {
    GPIO_SetBits(GPIOB, data);
}

// ---------------------------------------------------------------------------
// Clears the bits of PORTB according to "data"
// PORTB &= ~data
// ---------------------------------------------------------------------------

void gpio_clear(unsigned int data) {
    GPIO_ResetBits(GPIOB, data);
}

