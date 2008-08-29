/* ---------------------------------------------------------------------------
 * This file contains the queue management functions.
 * ------------------------------------------------------------------------ */

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <config.h>

/* ---------------------------------------------------------------------------
 * Enumeration of the return values of the queue functions
 * The queue can be "usable", full or empty
 * ------------------------------------------------------------------------ */

enum queue_result {QUEUE_OK, QUEUE_FULL, QUEUE_EMPTY};

/* ---------------------------------------------------------------------------
 * Type of the message in the queue (this is a 32 bit word by default
 * ------------------------------------------------------------------------ */

typedef struct {
    unsigned int command;
    unsigned int param;
} t_message;

typedef enum queue_result t_queue_result;

/* ---------------------------------------------------------------------------
 * If the size of the queue is not defined, the length will be 1 word
 * ------------------------------------------------------------------------ */

#ifndef QUEUE_SIZE
#define QUEUE_SIZE      1
#endif

/* ---------------------------------------------------------------------------
 * The queue contains the data field and two pointers: 
 * a read pointer, and a write pointer
 * ------------------------------------------------------------------------ */

struct queue {
    t_message data[QUEUE_SIZE + 1];
    unsigned int rd_ptr, wr_ptr;
};

typedef struct queue t_queue;

/* ---------------------------------------------------------------------------
 * Function to initialize the queue
 * Parameter: pointer to the queue
 * ------------------------------------------------------------------------ */

void queue_init(t_queue *);

/* ---------------------------------------------------------------------------
 * Puts new message into the queue.
 * If the queue if full, returns with an error value
 * Parameters:
 *      - pointer of the queue
 *      - pointer of the message that is to be sent
 * ------------------------------------------------------------------------ */

t_queue_result queue_put_non_blocking(t_queue *, t_message *);

/* ---------------------------------------------------------------------------
 * Gets new message feom the queue.
 * if the queue if empty, returns with an error value
 * Parameters:
 *      - pointer of the queue
 *      - pointer of the message (where the message will be stored)
 * ------------------------------------------------------------------------ */

t_queue_result queue_get_non_blocking(t_queue *, t_message *);

/* ---------------------------------------------------------------------------
 * Same as "queue_put_non_blocking", but it waits while the queue is full.
 * ------------------------------------------------------------------------ */

t_queue_result queue_put(t_queue *, t_message *);

/* ---------------------------------------------------------------------------
 * Same as "queue_get_non_blocking", but it waits while the queue is empty.
 * ------------------------------------------------------------------------ */

t_queue_result queue_get(t_queue *, t_message *);

#endif
