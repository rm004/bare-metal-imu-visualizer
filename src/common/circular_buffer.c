#include "common/circular_buffer.h"

bool circular_buffer_init(circular_buffer_t *buf, uint8_t size)
{
	bool result = false;

	if (buf && size > 0)
	{
		buf->size = size;
		buf->head = 0;
		buf->tail = 0;

		result = true;
	}

	return result;
}

bool circular_buffer_put(circular_buffer_t *buf, uint8_t data)
{
	bool result = false;

	if (buf && !circular_buffer_full(buf))
	{
		buf->buffer[buf->head] = data;
		buf->head++;

		// Avoid expensive modulo operation
		if (buf->head == buf->size)
		{
			buf->head = 0;
		}

		result = true;
	}

	return result;
}

bool circular_buffer_get(circular_buffer_t *buf, uint8_t *data)
{
	bool result = false;

	if (buf && !circular_buffer_empty(buf))
	{
		*data = buf->buffer[buf->tail];
		buf->tail++;

		// Avoid expensive modulo operation
		if (buf->tail == buf->size)
		{
			buf->tail = 0;
		}

		result = true;
	}

	return result;
}

bool circular_buffer_empty(const circular_buffer_t *buf)
{
	return buf->head == buf->tail;
}

bool circular_buffer_full(const circular_buffer_t *buf)
{
	uint8_t next_index = buf->head + 1;
	if (next_index == buf->size)
	{
		next_index = 0;
	}

	return next_index == buf->tail;
}
