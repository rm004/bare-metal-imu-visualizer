# Directories
CMSIS_INCLUDE_DIR = cmsis/Include
CMSIS_STM32F4XX_INCLUDE_DIR = cmsis/Device/ST/STM32F4xx/Include
SRC_HEADERS_DIR = inc
INCLUDE_DIRS = $(CMSIS_INCLUDE_DIR) $(CMSIS_STM32F4XX_INCLUDE_DIR) $(SRC_HEADERS_DIR)
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

# Toolchain
CC = arm-none-eabi-gcc
CPPCHECK = cppcheck

# Files
TARGET = $(BIN_DIR)/blinky
LINKER_SCRIPT = STM32F446RETX_FLASH.ld
C_SOURCES_WITH_HEADERS = \
	src/drivers/gpio.c
C_SOURCES = \
	src/main.c \
	src/system/syscalls.c \
	src/system/sysmem.c \
	src/system/system_stm32f4xx.c \
	$(C_SOURCES_WITH_HEADERS)
ASM_SOURCES = startup/startup_stm32f446retx.s
C_OBJECTS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(C_SOURCES))
ASM_OBJECTS = $(patsubst %.s, $(OBJ_DIR)/%.o, $(ASM_SOURCES))
HEADERS = $(patsubst src/%.c, inc/%.h, $(C_SOURCES_WITH_HEADERS))

# Flags
MCPU = cortex-m4
DEVICE = STM32F446xx
WFLAGS = -Wall -Wextra -Werror -Wshadow
CFLAGS = -mcpu=$(MCPU) -mthumb $(WFLAGS) $(addprefix -I, $(INCLUDE_DIRS)) -D$(DEVICE)
LDFLAGS = -mcpu=$(MCPU) -mthumb -T $(LINKER_SCRIPT)

VERBOSE ?= 0
ifeq ($(VERBOSE), 1)
Q := 
else
Q := @
endif

# Build
## Compiling .c files
$(OBJ_DIR)/%.o: %.c 
	$(Q)mkdir -p $(dir $@)
	$(Q)echo "CC $^ -> $@"
	$(Q)$(CC) $(CFLAGS) -c $^ -o $@

## Compiling .s files
$(OBJ_DIR)/%.o: %.s
	$(Q)mkdir -p $(dir $@)
	$(Q)echo "CC $^ -> $@"
	$(Q)$(CC) $(CFLAGS) -c $^ -o $@

## Linking
$(TARGET): $(C_OBJECTS) $(ASM_OBJECTS) $(HEADERS)
	$(Q)mkdir -p $(BIN_DIR)
	$(Q)echo "Linking -> $@"
	$(Q)$(CC) $(LDFLAGS) $^ -o $@

# Phonies
.PHONY: all clean flash cppcheck

all: $(TARGET)

clean:
	$(Q)rm -r $(BUILD_DIR)

flash: $(TARGET)
	openocd -f interface/stlink.cfg \
	-f target/stm32f4x.cfg \
	-c "program $(TARGET) verify reset exit"

cppcheck:
	$(Q)$(CPPCHECK) --quiet --enable=all --error-exitcode=1 \
	--inline-suppr \
	-I $(SRC_HEADERS_DIR) \
	$(C_SOURCES) \
	-i src/system \
	--suppress=missingInclude \
	--suppress=checkersReport