#include "queue.h"
#include <stdbool.h>
#include <stdlib.h>

Queue *AllocateQueue(void)
{
    Queue *queue = (Queue*) malloc(sizeof(Queue));
    if (queue == NULL)
    {
        return NULL;
    }
    queue->length = 0;
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

void FreeQueue(Queue *queue, void (*payloadFreeFn)(void*))
{
    if (queue == NULL)
    {
        return;
    }
    while (QueueHasNext(queue))
    {
        void *payload;
        PopQueue(queue, &payload);
        payloadFreeFn(payload);
    }
    free(queue);
}

bool QueueHasNext(Queue *queue)
{
    return queue != NULL && queue->length > 0;
}

bool PushQueue(Queue *queue, void *payload)
{
    if (queue == NULL)
    {
        return false;
    }
    QueueNode *node = (QueueNode*) malloc(sizeof(QueueNode));
    if (node == NULL)
    {
        return false;
    }
    node->next = NULL;
    node->payload = payload;
    if (queue->length > 0)
    {
        queue->tail->next = node;
    }
    else
    {
        queue->head = node;
    }
    queue->tail = node;
    queue->length++;
    return true;
}

bool PopQueue(Queue *queue, void **payload)
{
    if (!QueueHasNext(queue))
    {
        return false;
    }
    QueueNode *node = queue->head;
    queue->head = queue->head->next;
    queue->length--;
    *payload = node->payload;
    free(node);
    return true;
}
