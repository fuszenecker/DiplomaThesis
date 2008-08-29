#ifndef __TASKMAN_H__
#define __TASKMAN_H__


#include <config.h>
#include <sched.h>
#include <queue.h>
#include <serv_usart.h>

#define TM_INIT         0
#define TM_STOP         1
#define TM_READLINE     2
#define TM_READLINE2    3
#define TM_SEND         4
#define TM_PRINT_CH1    5
#define TM_PRINT_CH2    6
#define TM_LIST1        7
#define TM_LIST2        8
#define TM_LIST3        9
#define TM_LIST4        10
#define TM_LIST5        11
#define TM_LIST6        12

extern t_queue queue4taskman;
extern volatile unsigned int taskman_state;
void taskman();

#endif
