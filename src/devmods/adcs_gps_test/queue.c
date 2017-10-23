#include "queue.h"
#include <stdbool.h>
#include <stdint.h>

#include "core/utils.h"

Queue CreateQueue(uint8_t *contents, uint32_t itemSize, uint8_t capacity)
{
    Queue queue = { contents,
                    itemSize,
                    0,  // head
                    0,  // length
                    capacity };
    return queue;
}

FILE_STATIC uint8_t toArrayIndex(Queue *queue, uint8_t queueIndex)
{
    return (uint8_t) ((queue->head + queueIndex) % queue->capacity);
}

bool isQueueFull(Queue *queue)
{
    return queue->length >= queue->capacity;
}

bool isQueueEmpty(Queue *queue)
{
    return queue->length == 0;
}

uint8_t *WriteQueue(Queue *queue)
{
    if (queue == NULL || isQueueFull(queue))
    {
        return NULL;
    }
    return queue->contents
            + toArrayIndex(queue, queue->length) * queue->itemSize;
}

void PushQueue(Queue *queue)
{
    if (queue == NULL || isQueueFull(queue))
    {
        return;
    }
    queue->length++;
}

uint8_t *ReadQueue(Queue *queue)
{
    if (queue == NULL || isQueueEmpty(queue))
    {
        return NULL;
    }
    return queue->contents + queue->head * queue->itemSize;
}

void PopQueue(Queue *queue)
{
    if (isQueueEmpty(queue))
    {
        return;
    }

    queue->head++;
    if (queue->head >= queue->capacity)
    {
        queue->head = 0;
    }
    queue->length--;
}
