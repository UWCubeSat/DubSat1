#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

// please no accesserino
typedef struct queue
{
    uint8_t *contents;
    uint32_t itemSize;
    uint8_t head;
    uint8_t length;
    uint8_t capacity;
} Queue;

/**
 * Creates a Queue struct with populated values
 *
 * Arguments:
 *   -- contents: an array of length at least 'itemSize * capacity'
 *   -- itemSize: the constant size of each item stored in this queue
 *   -- capacity :the capacity of the queue
 *
 * Returns:
 *   an empty queue that can hold 'capacity' items each with size 'itemSize'
 */
Queue CreateQueue(uint8_t *contents, uint32_t itemSize, uint8_t capacity);

bool isQueueFull(Queue *queue);

bool isQueueEmpty(Queue *queue);

/**
 * Get the address of the next item to write. When the data on the address is
 * ready to read, call PushQueue
 *
 * Arguments:
 *   -- queue:
 *
 * Returns:
 *    -- the address of available memory of 'queue->itemSize' bytes,
 *    -- or NULL if the queue is NULL or full
 */
uint8_t *WriteQueue(Queue *queue);

/**
 * Make the data at the end of the queue available to read
 */
void PushQueue(Queue *queue);

/**
 * Get the address of the first item in the queue. Be sure to call PopQueue when
 * done reading the item.
 *
 * Arguments:
 *   -- queue:
 *
 * Returns:
 *   -- the address of the first item in the queue,
 *   -- or NULL if the queue is NULL or empty
 */
uint8_t *ReadQueue(Queue *queue);

/**
 * Free the first item in the queue and advance to the next
 */
void PopQueue(Queue *queue);

#endif /* QUEUE_H_ */
