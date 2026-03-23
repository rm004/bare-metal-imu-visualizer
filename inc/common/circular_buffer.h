#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdint.h>
#include <stdbool.h>

#define CIRCULAR_BUFFER_MAX_SIZE    UINT8_MAX

typedef struct
{
	uint8_t buffer[CIRCULAR_BUFFER_MAX_SIZE];
	uint8_t size;
	uint8_t head;
	uint8_t tail;
} circular_buffer_t;

bool circular_buffer_init(circular_buffer_t *buf, uint8_t size);
bool circular_buffer_put(circular_buffer_t *buf, uint8_t data);
bool circular_buffer_get(circular_buffer_t *buf, uint8_t *data);
bool circular_buffer_empty(const circular_buffer_t *buf);
bool circular_buffer_full(const circular_buffer_t *buf);

#endif // CIRCULAR_BUFFER_H