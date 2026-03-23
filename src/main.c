#include "drivers/gpio.h"
#include "common/pins.h"
#include "drivers/uart.h"
#include "stm32f4xx.h"

#include <stdio.h>
#include <stdbool.h>

int main(void)
{
	// Init LED pin
	gpio_init(&LED_PIN);

	// UART init
	uart_init();
	uint8_t data = 0;

	while (1)
	{
		bool read_success = uart_read(&data, 1);

		if (read_success)
		{
			printf("Received: %c\r\n", data);
		}
	}
}