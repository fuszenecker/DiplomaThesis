#ifndef __SERV_USART_H__
#define __SERV_USART_H__

#include <config.h>
#include <queue.h>
#include <gpio.h>
#include <serv_gpio.h>

extern t_queue queue4usart;

#define USART_INIT  0
#define USART_SEND  1
#define USART_RECV  2
#define USART_PRINT 3
#define USART_RECVD 0xffff0001

void serv_usart();

#endif
