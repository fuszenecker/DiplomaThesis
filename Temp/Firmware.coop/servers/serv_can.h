#ifndef __SERV_CAN_H__
#define __SERV_CAN_H__

#include <config.h>
#include <queue.h>
#include <can.h>
#include <serv_can.h>

extern t_queue queue4can;

#define CAN_INIT    0
#define CAN_SEND    1
#define CAN_RECV    2
#define CAN_DATA0   3
#define CAN_DATA1   4
#define CAN_ADDR    5

void serv_can();

#endif
