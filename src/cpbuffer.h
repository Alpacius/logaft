#pragma once

// Comes from cleanwater.

#include    "stdc_common.h"
#include    "list.h"

struct cpbuffer {
    uint8_t index_producer;
    uint8_t consuming;
    struct link_index buffers[2];
};

#define     CP_BUFFER_CONSUMING                  0
#define     CP_BUFFER_FREE                       1      // XXX Do NOT check this constant

#define     CP_BUFFER_PRODUCER_BUSY              0x2
#define     CP_BUFFER_INITIAL_PRODUCER_INDEX     0x0

#define     cp_buffer_normalized_index(index_)   ((index_) & 0x1)
#define     cp_buffer_flipped_index(index_)      (1 - cp_buffer_normalized_index((index_)))
#define     cp_buffer_flipped_index_raw(index_)  (1 - (index_))
#define     cp_buffer_consumer_index(queue_)     (1 - cp_buffer_normalized_index((queue_)->index_producer))

#define     cp_buffer_index_of(queue_)           __atomic_load_n(&((queue_)->index_producer), __ATOMIC_ACQUIRE)

// Beware: the consumer may fail to refresh its buffer.
static inline
struct link_index *cp_buffer_consume(struct cpbuffer *queue) {
    if (list_is_empty(&(queue->buffers[cp_buffer_consumer_index(queue)]))) {
        uint8_t desired_index = cp_buffer_normalized_index(cp_buffer_index_of(queue));
        queue->consuming = __atomic_compare_exchange_n(&(queue->index_producer), &desired_index, cp_buffer_flipped_index_raw(desired_index), 0, __ATOMIC_ACQ_REL, __ATOMIC_RELAXED);
        return queue->consuming ?
            &(queue->buffers[cp_buffer_flipped_index(queue->index_producer)]) :
            NULL;
    }
    return &(queue->buffers[cp_buffer_consumer_index(queue)]);
}

static inline
void cp_buffer_produce(struct cpbuffer *queue, struct link_index *product) {
    __atomic_or_fetch(&(queue->index_producer), CP_BUFFER_PRODUCER_BUSY, __ATOMIC_RELEASE);
    {
        struct link_index *target_queue = &(queue->buffers[cp_buffer_normalized_index(cp_buffer_index_of(queue))]);
        list_add_tail(product, target_queue);
    }
    __atomic_and_fetch(&(queue->index_producer), ~CP_BUFFER_PRODUCER_BUSY, __ATOMIC_RELEASE);
}

static inline
struct cpbuffer *cp_buffer_init(struct cpbuffer *queue) {
    queue->consuming = CP_BUFFER_FREE;
    __atomic_store_n(&(queue->index_producer), CP_BUFFER_INITIAL_PRODUCER_INDEX, __ATOMIC_RELEASE);
    list_init(&(queue->buffers[0]));
    list_init(&(queue->buffers[1]));
}

#undef      CP_BUFFER_PRODUCER_BUSY
#undef      CP_BUFFER_INITIAL_PRODUCER_INDEX

#undef      cp_buffer_normalized_index
#undef      cp_buffer_flipped_index
#undef      cp_buffer_flipped_index_raw
#undef      cp_buffer_consumer_index

#undef      cp_buffer_index_of
