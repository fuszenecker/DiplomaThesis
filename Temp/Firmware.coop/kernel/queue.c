/* ---------------------------------------------------------------------------
 * This file contains the queue management functions.
 * ------------------------------------------------------------------------ */

#include <queue.h>

/* ---------------------------------------------------------------------------
 * Function to initialize the queue
 * Parameter: pointer to the queue
 * ------------------------------------------------------------------------ */

void queue_init (t_queue *queue) {
    queue->rd_ptr = 0;
    queue->wr_ptr = 0;
}

/* ---------------------------------------------------------------------------
 * Puts new message into the queue.
 * If the queue if full, returns with an error value
 * Parameters:
 *      - pointer of the queue
 *      - pointer of the message that is to be sent
 * ------------------------------------------------------------------------ */

t_queue_result queue_put_non_blocking(t_queue *queue, t_message *msg) {
    unsigned int i;

    if (((queue->wr_ptr + 1) % (QUEUE_SIZE + 1)) == queue->rd_ptr) {
        return QUEUE_FULL;
    } else {
        queue->wr_ptr++;
        queue->wr_ptr %= (QUEUE_SIZE + 1);

        for (i = 0; i < sizeof(t_message); i++)
            ((char *)&(queue->data[queue->wr_ptr]))[i] = ((char *) msg)[i];

        return QUEUE_OK;
    }
}

/* ---------------------------------------------------------------------------
 * Gets new message feom the queue.
 * if the queue if empty, returns with an error value
 * Parameters:
 *      - pointer of the queue
 *      - pointer of the message (where the message will be stored)
 * ------------------------------------------------------------------------ */

t_queue_result queue_get_non_blocking(t_queue *queue, t_message *msg) {
    unsigned int i;

    if (queue->rd_ptr == queue->wr_ptr) {
        return QUEUE_EMPTY;
    } else {
        queue->rd_ptr++;
        queue->rd_ptr %= (QUEUE_SIZE + 1);

        for (i = 0; i < sizeof(t_message); i++)
            ((char *)msg)[i] = ((char *)(&(queue->data[queue->rd_ptr])))[i];

        return QUEUE_OK;
    }
}

/* ---------------------------------------------------------------------------
 * Same as "queue_put_non_blocking", but it waits while the queue is full.
 * ------------------------------------------------------------------------ */

t_queue_result queue_put(t_queue *queue, t_message *msg) {
    while (queue_put_non_blocking(queue, msg) == QUEUE_FULL);
    return QUEUE_OK;
}

/* ---------------------------------------------------------------------------
 * Same as "queue_get_non_blocking", but it waits while the queue is empty.
 * ------------------------------------------------------------------------ */

t_queue_result queue_get(t_queue *queue, t_message *msg) {
    while (queue_get_non_blocking(queue, msg) == QUEUE_EMPTY);
    return QUEUE_OK;
}

