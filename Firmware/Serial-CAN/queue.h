#ifndef __QUEUE_H__
#define __QUEUE_H__

typedef enum {QUEUE_OK, QUEUE_FULL, QUEUE_EMPTY} t_queue_result;

typedef struct {
    unsigned int command; 
    unsigned int param1;
    unsigned int param2;
    unsigned int param3;
} t_message;

#ifndef QUEUE_SIZE
#define QUEUE_SIZE      1
#endif

typedef struct {
    t_message data[QUEUE_SIZE];
    unsigned int rd_ptr, wr_ptr;
} t_queue;

void queue_init(t_queue *);
t_queue_result queue_put_non_blocking(t_queue *, t_message *);
t_queue_result queue_get_non_blocking(t_queue *, t_message **);
t_queue_result queue_remove_non_blocking(t_queue *);
t_queue_result queue_put(t_queue *, t_message *);
t_queue_result queue_get(t_queue *, t_message **);
t_queue_result queue_remove(t_queue *);

#endif
