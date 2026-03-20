# Directories
CMSIS_INCLUDE_DIR = cmsis/Include
CMSIS_STM32F4XX_INCLUDE_DIR = cmsis/Device/ST/STM32F4xx/Include
SRC_HEADERS = inc
INCLUDE_DIRS = $(CMSIS_INCLUDE_DIR) $(CMSIS_STM32F4XX_INCLUDE_DIR) $(SRC_HEADERS)
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

#Toolchain
CC = arm-none-eabi-gcc

#Files
TARGET = $(BIN_DIR)/blinky
LINKER_SCRIPT = STM32F446RETX_FLASH.ld
C_SOURCES = src/main.c \
		  src/system/syscalls.c \
		  src/system/sysmem.c \
		  src/system/system_stm32f4xx.c \
		  src/drivers/gpio.c
ASM_SOURCES = startup/startup_stm32f446retx.s
C_OBJECTS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(C_SOURCES))
ASM_OBJECTS = $(patsubst %.s, $(OBJ_DIR)/%.o, $(ASM_SOURCES))

# Flags
MCPU = cortex-m4
DEVICE = STM32F446xx
WFLAGS = -Wall -Wextra -Werror -Wshadow
CFLAGS = -mcpu=$(MCPU) -mthumb $(WFLAGS) $(addprefix -I, $(INCLUDE_DIRS)) -D $(DEVICE)
LDFLAGS = -mcpu=$(MCPU) -mthumb -T $(LINKER_SCRIPT)

VERBOSE ?= 0
ifeq ($(VERBOSE), 1)
Q := 
else
Q := @
endif

$(OBJ_DIR)/%.o: %.c 
	$(Q)mkdir -p $(dir $@)
	$(Q)echo "CC $^ -> $@"
	$(Q)$(CC) $(CFLAGS) -c $^ -o $@

$(OBJ_DIR)/%.o: %.s
	$(Q)mkdir -p $(dir $@)
	$(Q)echo "CC $^ -> $@"
	$(Q)$(CC) $(CFLAGS) -c $^ -o $@

$(TARGET): $(C_OBJECTS) $(ASM_OBJECTS)
	$(Q)mkdir -p $(BIN_DIR)
	$(Q)echo "Linking -> $@"
	$(Q)$(CC) $(LDFLAGS) $^ -o $@

.PHONY: clean flash final

final: $(TARGET)

flash: $(TARGET)
	openocd -f interface/stlink.cfg \
	-f target/stm32f4x.cfg \
	-c "program $(TARGET) verify reset exit"

clean:
	$(Q)rm -r $(BIN_DIR)/* $(OBJ_DIR)/*