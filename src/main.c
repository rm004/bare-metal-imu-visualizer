#include "drivers/gpio.h"
#include "common/pins.h"
#include "stm32f4xx.h"

int main(void)
{
	// Init LED pin
	gpio_init(&LED_PIN);

	while (1)
	{
		// Toggle LED
		gpio_toggle(&LED_PIN);
		for (int i = 0; i < 100000; i++) {}
	}
}