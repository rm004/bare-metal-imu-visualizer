#include "drivers/uart.h"
#include "drivers/gpio.h"
#include "stm32f4xx.h"

#include <stdint.h>

#define USART2_CLK_EN      (1u << 17u)
#define UART_BARDRATE      115200u
#define SYS_CLK            16000000u
#define APB1_CLK           SYS_CLK
#define UART_CR1_M         (1u << 12u)
#define UART_CR1_TE        (1u << 3u)
#define UART_CR1_RE        (1u << 2u)
#define UART_CR1_UE        (1u << 13u)
#define UART_CR1_RXNEIE    (1u << 5u)

static void configure_pins(void);
static void configure_uart(void);
static uint16_t compute_uart_baudrate(uint32_t periph_clk, uint32_t baudrate);

void uart_init(void)
{
	configure_pins();
	configure_uart();
}

// void uart_write(uint8_t *data, const uint32_t length)
// {

// }

// void uart_write_byte(uint8_t data)
// {

// }

// uint32_t uart_read(uint8_t *data, const uint32_t length)
// {

// }

// uint8_t uart_read_byte(void)
// {

// }

// bool uart_data_available(void)
// {

// }

void USART2_IRQHandler(void)
{

}

static void configure_pins(void)
{	
	// Configure PA2 (USART2 TX pin)
	const gpio_t uart2_tx =
	{
		.port = GPIOA,
		.pin = GPIO_PIN_2,
		.config =
		{
			.mode = GPIO_MODE_ALT_FUNC,
			.output_type = GPIO_OUTPUT_TYPE_PUSH_PULL,
			.speed = GPIO_SPEED_LOW,
			.resist = GPIO_RESIST_NONE,
			.alt_func = GPIO_ALT_FUNC_7
		}
	};

	// Configure PA3 (USART2 RX pin)
	const gpio_t uart2_rx =
	{
		.port = GPIOA,
		.pin = GPIO_PIN_3,
		.config =
		{
			.mode = GPIO_MODE_ALT_FUNC,
			.output_type = GPIO_OUTPUT_TYPE_PUSH_PULL,
			.speed = GPIO_SPEED_LOW,
			.resist = GPIO_RESIST_NONE,
			.alt_func = GPIO_ALT_FUNC_7
		}
	};

	// Initialize TX/RX pins with configs
	gpio_init(&uart2_tx);
	gpio_init(&uart2_rx);
}

static void configure_uart(void)
{
	// Enable clock for UART2
	RCC->APB1ENR |= USART2_CLK_EN;

	// Set word length to 8 data bits
	USART2->CR1 &= ~(UART_CR1_M);

	// Set the baud rate
	USART2->BRR = compute_uart_baudrate(APB1_CLK, UART_BARDRATE);

	// Enable the transmitter to send an idle frame as first transmission.
	USART2->CR1 = UART_CR1_TE;

	// Enable the receiver
	USART2->CR1 |= UART_CR1_RE;

	// Enable USART2
	USART2->CR1 |= UART_CR1_UE;

	// Enable RXNE interrupt
	USART2->CR1 |= UART_CR1_RXNEIE;

	// Enable interrupts for USART2
	NVIC_EnableIRQ(USART2_IRQn);
}

static uint16_t compute_uart_baudrate(uint32_t periph_clk, uint32_t baudrate)
{
	return ((periph_clk + (baudrate / 2u)) / baudrate);
}