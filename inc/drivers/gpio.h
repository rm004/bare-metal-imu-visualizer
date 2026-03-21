#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "stm32f4xx.h"

typedef enum
{
	GPIO_PIN_0 = 0x0u,
	GPIO_PIN_1,
	GPIO_PIN_2,
	GPIO_PIN_3,
	GPIO_PIN_4,
	GPIO_PIN_5,
	GPIO_PIN_6,
	GPIO_PIN_7,
	GPIO_PIN_8,
	GPIO_PIN_9,
	GPIO_PIN_10,
	GPIO_PIN_11,
	GPIO_PIN_12,
	GPIO_PIN_13,
	GPIO_PIN_14,
	GPIO_PIN_15
} gpio_pin_t;

typedef enum
{
	GPIO_OUTPUT_TYPE_PUSH_PULL = 0x0u,
	GPIO_OUTPUT_TYPE_OPEN_DRAIN
} gpio_output_type_t;

typedef enum
{
	GPIO_SPEED_LOW = 0x0u,
	GPIO_SPEED_MED,
	GPIO_SPEED_FAST,
	GPIO_SPEED_HIGH
} gpio_speed_t;

typedef enum
{
	GPIO_MODE_INPUT = 0x0u,
	GPIO_MODE_OUPUT,
	GPIO_MODE_ALT_FUNC,
	GPIO_MODE_ANALOG
} gpio_mode_t;

typedef enum
{
	GPIO_RESIST_NONE = 0x0u,
	GPIO_RESIST_PULL_UP,
	GPIO_RESIST_PULL_DOWN,
	GPIO_RESIST_RESERVED
} gpio_resist_t;

typedef enum
{
	GPIO_ALT_FUNC_0 = 0x0u,
	GPIO_ALT_FUNC_1,
	GPIO_ALT_FUNC_2,
	GPIO_ALT_FUNC_3,
	GPIO_ALT_FUNC_4,
	GPIO_ALT_FUNC_5,
	GPIO_ALT_FUNC_6,
	GPIO_ALT_FUNC_7,
	GPIO_ALT_FUNC_8,
	GPIO_ALT_FUNC_9,
	GPIO_ALT_FUNC_10,
	GPIO_ALT_FUNC_11,
	GPIO_ALT_FUNC_12,
	GPIO_ALT_FUNC_13,
	GPIO_ALT_FUNC_14,
	GPIO_ALT_FUNC_15
} gpio_alt_func_t;

typedef enum
{
	GPIO_LOW = 0x0u,
	GPIO_HIGH
} gpio_state_t;

typedef struct
{
	gpio_mode_t mode;
	gpio_output_type_t output_type;
	gpio_speed_t speed;
	gpio_resist_t resist;
	gpio_alt_func_t alt_func;
} gpio_config_t;

typedef struct
{
	GPIO_TypeDef *port;
	gpio_pin_t pin;
	gpio_config_t config;
} gpio_t;

void gpio_init(const gpio_t *gpio);
void gpio_write(const gpio_t *gpio, gpio_state_t state);
void gpio_toggle(const gpio_t *gpio);
gpio_state_t gpio_read(const gpio_t *gpio);

#endif // GPIO_H