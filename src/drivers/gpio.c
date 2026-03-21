#include "drivers/gpio.h"

static uint8_t gpio_port_index(const GPIO_TypeDef *port)
{
	if (port == GPIOA) return 0u;
	if (port == GPIOB) return 1u;
	if (port == GPIOC) return 2u;
	if (port == GPIOD) return 3u;
	if (port == GPIOE) return 4u;
	if (port == GPIOF) return 5u;
	if (port == GPIOG) return 6u;
	if (port == GPIOH) return 7u;

	return 0u;
}

void gpio_init(const gpio_t *gpio)
{
	gpio_pin_t pin = gpio->pin;
	GPIO_TypeDef *port = gpio->port;

	// Enable clock access to port
	RCC->AHB1ENR |= (0x1u << gpio_port_index(port));

	// Clear and set mode
	port->MODER &= ~(0x3u << (pin * 2u));
	port->MODER |= (gpio->config.mode << (pin * 2u));

	// Clear and set output type
	port->OTYPER &= ~(0x1u << pin);
	port->OTYPER |= (gpio->config.output_type << pin);

	// Clear and set speed
	port->OSPEEDR &= ~(0x3u << (pin * 2u));
	port->OSPEEDR |= (gpio->config.speed << (pin * 2u));

	// Clear and set resistor
	port->PUPDR &= ~(0x3u << (pin * 2u));
	port->PUPDR |= (gpio->config.resist << (pin * 2u));

	// Clear and set alternate function
	if (gpio->config.mode == GPIO_MODE_ALT_FUNC)
	{
		uint8_t alt_func_index = pin <= 7u ? 0u : 1u;
		port->AFR[alt_func_index] &= ~(0xFu << (pin * 4u));
		port->AFR[alt_func_index] |= (gpio->config.alt_func << (pin * 4u));
	}
}

void gpio_write(const gpio_t *gpio, gpio_state_t state)
{
	if (gpio->config.mode == GPIO_MODE_OUPUT)
	{
		gpio->port->BSRR = (0x1u << (state == GPIO_HIGH ? gpio->pin : (gpio->pin + 16u)));
	}
}

void gpio_toggle(const gpio_t *gpio)
{
	gpio_state_t new_state = ((gpio->port->ODR & (0x1u << gpio->pin)) != 0u) ? GPIO_LOW : GPIO_HIGH;
	gpio_write(gpio, new_state);
}

gpio_state_t gpio_read(const gpio_t *gpio)
{
	return ((gpio->port->IDR & (0x1u << gpio->pin)) != 0u) ? GPIO_HIGH : GPIO_LOW;
}