final: blinky.elf

main.o: src/main.c
	arm-none-eabi-gcc -I cmsis/Device/ST/STM32F4xx/Include/ -I cmsis/Include -I inc -D STM32F446xx -c -mcpu=cortex-m4 -mthumb src/main.c -o build/obj/main.o

syscalls.o: src/system/syscalls.c
	arm-none-eabi-gcc -I inc -c -mcpu=cortex-m4 -mthumb src/system/syscalls.c -o build/obj/syscalls.o

sysmem.o: src/system/sysmem.c
	arm-none-eabi-gcc -I inc -c -mcpu=cortex-m4 -mthumb src/system/sysmem.c -o build/obj/sysmem.o

system_stm32f4xx.o: src/system/system_stm32f4xx.c
	arm-none-eabi-gcc -I cmsis/Device/ST/STM32F4xx/Include/ -I cmsis/Include -I inc -D STM32F446xx -c -mcpu=cortex-m4 -mthumb src/system/system_stm32f4xx.c -o build/obj/system_stm32f4xx.o

gpio.o: src/drivers/gpio.c
	arm-none-eabi-gcc -I inc -c -mcpu=cortex-m4 -mthumb src/drivers/gpio.c -o build/obj/gpio.o

startup_stm32f446retx.o: startup/startup_stm32f446retx.s
	arm-none-eabi-gcc -c -mcpu=cortex-m4 -mthumb startup/startup_stm32f446retx.s -o build/obj/startup_stm32f446retx.o

blinky.elf: main.o gpio.o syscalls.o sysmem.o system_stm32f4xx.o startup_stm32f446retx.o
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -T STM32F446RETX_FLASH.ld build/obj/*.o -o build/bin/blinky.elf -Wl,-Map=build/bin/blinky.map

flash: blinky.elf
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
	-c "program build/bin/blinky.elf verify reset exit"

clean:
	rm -r build/bin/* build/obj/*