#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdbool.h>

void uart_init(void);
bool uart_write(const uint8_t *data, uint8_t length);
bool uart_read(uint8_t *data, uint8_t length);

#endif // UART_H