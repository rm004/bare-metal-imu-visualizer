#include "drivers/uart.h"
#include "drivers/gpio.h"
#include "common/circular_buffer.h"
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
#define UART_CR1_TXEIE     (1u << 7u)
#define UART_SR_TXE        (1u << 7u)
#define UART_SR_RXNE       (1u << 5u)

static circular_buffer_t tx_buf;
static circular_buffer_t rx_buf;

static void configure_pins(void);
static void configure_uart(void);
static uint16_t compute_uart_baudrate(uint32_t periph_clk, uint32_t baudrate);

int __io_putchar(int ch)
{
	const uint8_t data[1] = {ch};

	uart_write(data, 1);

	return ch;
}

void uart_init(void)
{
	circular_buffer_init(&tx_buf, CIRCULAR_BUFFER_MAX_SIZE);
	circular_buffer_init(&rx_buf, CIRCULAR_BUFFER_MAX_SIZE);

	configure_pins();
	configure_uart();
}

bool uart_write(const uint8_t *data, uint8_t length)
{
	uint8_t bytes_written = 0;

	if (data && length > 0)
	{
		for (uint8_t i = 0; i < length; i++)
		{
			// Make sure shared buffer write is not interrupted
			NVIC_DisableIRQ(USART2_IRQn);
			bool write_result = circular_buffer_put(&tx_buf, data[i]);
			NVIC_EnableIRQ(USART2_IRQn);
			
			if (write_result)
			{
				bytes_written++;
			}
			else
			{
				break;
			}
		}

		if (bytes_written > 0)
		{
			// Enable TXE interrupts to send the data in the buffer
			USART2->CR1 |= UART_CR1_TXEIE;
		}
	}

	return bytes_written == length;
}

bool uart_read(uint8_t *data, uint8_t length)
{
	uint8_t bytes_read = 0;

	uint8_t byte = 0;

	while (bytes_read < length)
	{
		NVIC_DisableIRQ(USART2_IRQn);
		bool read_result = circular_buffer_get(&rx_buf, &byte);
		NVIC_EnableIRQ(USART2_IRQn);

		if (read_result)
		{
			data[bytes_read] = byte;
			bytes_read++;
		}
		else
		{
			break;
		}
	}

	return bytes_read == length;
}

void USART2_IRQHandler(void)
{
	if ((USART2->SR & UART_SR_RXNE) && (USART2->CR1 & UART_CR1_RXNEIE))
	{
		uint8_t data = USART2->DR;

		circular_buffer_put(&rx_buf, data);
	}

	if ((USART2->SR & UART_SR_TXE) && (USART2->CR1 & UART_CR1_TXEIE))
	{
		uint8_t data = 0;

		bool get_success = circular_buffer_get(&tx_buf, &data);

		if (get_success)
		{
			USART2->DR = data;
		}

		if (!get_success || circular_buffer_empty(&tx_buf))
		{
			// Disable TXE interrupt because all data has been sent
			USART2->CR1 &= ~(UART_CR1_TXEIE);
		}
	}
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
	uint32_t usartdiv_times_16 = (periph_clk + (baudrate / 2u)) / baudrate;
    return (uint16_t)usartdiv_times_16;
}