#ifndef __SERV_GPIO_H__
#define __SERV_GPIO_H__

#include <config.h>
#include <queue.h>
#include <gpio.h>
#include <serv_gpio.h>

#define GPIO_INIT   0
#define GPIO_SET    1
#define GPIO_CLEAR  2

extern t_queue queue4gpio;

void serv_gpio();

#endif
