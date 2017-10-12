#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdbool.h>
#include <stdlib.h>

typedef struct q_node
{
    void *payload;
    struct q_node *next;
} QueueNode;

typedef struct queue
{
    size_t length;
    QueueNode *head;
    QueueNode *tail;
} Queue;

Queue *AllocateQueue(void);

void FreeQueue(Queue *queue, void (*payloadFreeFn)(void *payload));

bool QueueHasNext(Queue *queue);

bool PushQueue(Queue *queue, void *payload);

bool PopQueue(Queue *queue, void **payload);

#endif /* QUEUE_H_ */
