#include <queue.h>

void queue_init (t_queue *queue) {
    queue->rd_ptr = 0;
    queue->wr_ptr = 0;
}

t_queue_result queue_put_non_blocking(t_queue *queue, t_message *msg) {
    unsigned int k;

    if ((queue->wr_ptr != queue->rd_ptr) && ((queue->wr_ptr % QUEUE_SIZE) == (queue->rd_ptr % QUEUE_SIZE))) {
        return QUEUE_FULL;
    } else {
        for (k = 0; k < sizeof(t_message); k++)
            ((char *)(&(queue->data[queue->wr_ptr % QUEUE_SIZE])))[k] = ((char *)msg)[k];

        queue->wr_ptr++;

        return QUEUE_OK;
    }
}

t_queue_result queue_get_non_blocking(t_queue *queue, t_message **msg) {
    if (queue->rd_ptr == queue->wr_ptr) {
        return QUEUE_EMPTY;
    } else {
        *msg = &(queue->data[queue->rd_ptr % QUEUE_SIZE]);
        return QUEUE_OK;
    }
}

t_queue_result queue_remove_non_blocking(t_queue *queue) {
    if (queue->rd_ptr == queue->wr_ptr) {
        return QUEUE_EMPTY;
    } else {
        queue->rd_ptr++;
        return QUEUE_OK;
    }
}

t_queue_result queue_put(t_queue *queue, t_message *msg) {
    while (queue_put_non_blocking(queue, msg) == QUEUE_FULL);
    return QUEUE_OK;
}

t_queue_result queue_get(t_queue *queue, t_message **msg) {
    while (queue_get_non_blocking(queue, msg) == QUEUE_EMPTY);
    return QUEUE_OK;
}

t_queue_result queue_remove(t_queue *queue) {
    while (queue_remove_non_blocking(queue) == QUEUE_EMPTY);
    return QUEUE_OK;
}

