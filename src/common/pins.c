#include "common/pins.h"

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