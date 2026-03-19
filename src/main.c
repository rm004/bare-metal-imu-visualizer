#include "drivers/gpio.h"
#include "stm32f4xx.h"

#define GPIOA_EN    (1u << 0)
#define PIN5        (5)
#define LED_PIN     (1u << 5)

int main(void)
{
	print_hello();

	RCC->AHB1ENR |= GPIOA_EN;

	GPIOA->MODER |= (1u << (PIN5 * 2u));
	GPIOA->MODER &= ~(1u << ((PIN5 * 2u) + 1u));

	while (1)
	{
		GPIOA->ODR ^= LED_PIN;
		for (int i = 0; i < 100000; i++) {}
	}
}