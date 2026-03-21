#include "drivers/gpio.h"
#include "stm32f4xx.h"

int main(void)
{
	// Congigure LED pin
	const gpio_t LED_PIN =
	{
		.port = GPIOA,
		.pin = GPIO_PIN_5,
		.config =
		{
			.mode = GPIO_MODE_OUPUT,
			.output_type = GPIO_OUTPUT_TYPE_PUSH_PULL,
			.speed = GPIO_SPEED_LOW,
			.resist = GPIO_RESIST_NONE,
			.alt_func = GPIO_ALT_FUNC_0
		}
	};

	// Init LED pin
	gpio_init(&LED_PIN);

	while (1)
	{
		// Toggle LED
		gpio_toggle(&LED_PIN);
		for (int i = 0; i < 100000; i++) {}
	}
}