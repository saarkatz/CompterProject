#ifndef SPBPRIORITYQUEUE_H_
#define SPBPRIORITYQUEUE_H_
#include <stdbool.h>

/**
 * SP Bounded Priority Queue summary
 *
 * Holds a number of elements up to a certain max size. Each element keeps
 * a non-negative integer index and a non-negative double priority value.
 * Whenever a new element is added to the queue, if the queue is at full
 * capacity, the element with the highest priority value is ejected from
 * the queue.
 *
 * The following functions are supported:
 *
 * spBPQueueCreate      - Creates a new queue
 * spBPQueueCopy        - Creates a new copy of a given queue
 * spBPQueueDestroy     - Free all resources associated with a queue
 * spBPQueueClear       - Remove all elements from a queue
 * spBPQueueSize        - Returns the size of a queue
 * spBPQueueGetMaxSize  - Returns maximum capacity of a queue
 * spBPQueueEnqueue     - Add an element with the given data to the queue
 * spBPQueueDequeue     - Removes the element with the smallest value from the 
 *                        queue
 * spBPQueuePeek        - Sets BPQueueElement to the data of the smallest
 *                        element
 * spBPQueuePeekLast    - Sets BPQueueElement to the data of the largest
 *                        element
 * spBPQueueMinValue    - Returns the minimum value within the queue
 * spBPQueueMaxValue    - Returns the maximum value within the queue
 * spBPQueueIsEmpty     - Returns true if the queue is empty
 * spBPQueueIsFull      - Returns true if the queue is full
 */


/** type used to define Bounded priority queue **/
typedef struct sp_bp_queue_t SPBPQueue;

typedef struct sp_bpq_element_t {
	int index;
	double value;
} BPQueueElement;

/** type for error reporting **/
typedef enum sp_bp_queue_msg_t {
	SP_BPQUEUE_OUT_OF_MEMORY,
	SP_BPQUEUE_FULL,
	SP_BPQUEUE_EMPTY,
	SP_BPQUEUE_INVALID_ARGUMENT,
	SP_BPQUEUE_SUCCESS
} SP_BPQUEUE_MSG;

/**
 * Allocates a new BPQueue in memory.
 * Given a maxSize, the capacity of the queue will be set to maxSize.
 *
 * @return
 * NULL in case allocation failure ocurred OR maxSize <=0
 * Otherwise, the new queue is returned
 */
SPBPQueue* spBPQueueCreate(int maxSize);

/**
 * Allocates a copy of the given queue.
 *
 * Given the queue source, the functions returns a new queue Q such that:
 * - Foreach element in source there exists a copy in Q with the same index
 *   and value.
 * 
 * @param source - The source queue
 * @assert (source != NULL)
 * @return
 * NULL in case memory allocation failure occurs
 * Otherwise a copy of source is returned.
 */
SPBPQueue* spBPQueueCopy(SPBPQueue* source);

/**
 * Free all memory allocation associated with queue,
 * if queue is NULL nothing happens.
 */
void spBPQueueDestroy(SPBPQueue* queue);

/**
 * Removes all the elements from queue.
 *
 * @param queue - The source queue
 * @assert (queue != NULL)
 */
void spBPQueueClear(SPBPQueue* queue);

/**
 * Returns the current size of the queue.
 *
 * @param queue - The source queue
 * @assert (queue != NULL)
 * @return The size of the queue.
 */
int spBPQueueSize(SPBPQueue* queue);

/**
 * Returns the maximum capacity of the queue.
 *
 * @param queue - The source queue
 * @assert (queue != NULL)
 * @return The capacity of the queue.
 */
int spBPQueueGetMaxSize(SPBPQueue* queue);

/**
 * Enqueues a new element with the index and value specified.
 * If the queue exceeds it's maximum capacity due to the insertion, the element
 * with the largest priority value is ejected from the queue. In this case
 * SP_BPQUEUE_FULL is returned.
 *
 * @param queue - The source queue
 * @param index - The index of the new element
 * @param value - The priority of the new value
 * @assert (queue != NULL)
 * @return
 * SP_BPQUEUE_INVALID_ARGUMENT if index < 0 OR value < 0.0
 * SP_BPQUEUE_FULL if the queue is full prior to the enqueue. The new element
 * will still be inserted while the last element is ejected from the queue.
 * Otherwise SP_BPQUEUE_SUCCESS.
 */
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue* queue, int index, double value);

/**
 * Dequeues the element with the minimum value, removing it from the queue.
 * If the queue is empty SP_BPQUEUE_EMPTY is returned and nothing else happens.
 *
 * @param queue - The source queue
 * @assert (queue != NULL)
 * @return
 * SP_BPQUEUE_EMPTY If the queue is empty prior to the dequeue.
 * Otherwise SP_BPQUEUE_SUCCESS.
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue* queue);

/**
 * Sets res to contain the data of the element with the minimum priority value.
 * res is set to NULL if queue is empty.
 *
 * @param queue - The source queue
 * @param res - The result of the peek
 * @assert (queue != NULL)
 * @assert (res != NULL)
 * @return 
 * SP_BPQUEUE_EMPTY If the queue is empty prior to the dequeue.
 * Otherwise SP_BPQUEUE_SUCCESS.
 */
SP_BPQUEUE_MSG spBPQueuePeek(SPBPQueue* queue, BPQueueElement* res);

/**
 * Sets res to contain the data of the element with the maximum priority value.
 * res is set to NULL if queue is empty.
 *
 * @param queue - The source queue
 * @param res - The result of the peek
 * @assert (queue != NULL)
 * @return
 * SP_BPQUEUE_EMPTY If the queue is empty prior to the dequeue.
 * Otherwise SP_BPQUEUE_SUCCESS.
 */
SP_BPQUEUE_MSG spBPQueuePeekLast(SPBPQueue* queue, BPQueueElement* res);

/**
 * Returns the minimum priority value within the queue. Returns -1.0 if the
 * queue is empty.
 *
 * @param queue - The source queue
 * @assert (queue != NULL)
 * @return The minimum value in the queue. -1.0 if the queue is empty.
 */
double spBPQueueMinValue(SPBPQueue* queue);

/**
* Returns the maximum priority value within the queue. Returns -1.0 if the
* queue is empty.
*
* @param queue - The source queue
* @assert (queue != NULL)
* @return The maximum value in the queue. -1.0 if the queue is empty.
*/
double spBPQueueMaxValue(SPBPQueue* queue);

/**
 * Returns true if the queue is empty, otherwise returns false.
 *
 * @param queue - The source queue
 * @assert (queue != NULL)
 * @return True if the queue is empty, false otherwise.
 */
bool spBPQueueIsEmpty(SPBPQueue* queue);

/**
* Returns true if the queue is full, otherwise returns false.
*
* @param queue - The source queue
* @assert (queue != NULL)
* @return True if the queue is full, false otherwise.
*/
bool spBPQueueIsFull(SPBPQueue* queue);

#endif // SPBPRIORITYQUEUE_H_
