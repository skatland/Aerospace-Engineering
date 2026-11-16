# Shared build rules. Every lesson has a tiny Makefile that just says:
#     include ../../common/Makefile.inc

CC      = arm-none-eabi-gcc
CFLAGS  = -mcpu=cortex-m3 -mthumb -O0 -g -Wall -ffreestanding -I. -I../../common
LDFLAGS = -T ../../common/linker.ld -nostdlib -Wl,-Map=main.map

SRCS        ?= main.c
STARTUP     ?= ../../common/startup.c
SOL_SRCS    ?= solution/main.c
SOL_STARTUP ?= $(STARTUP)
COMMON       = ../../common/uart.c

main.elf: $(SRCS) $(STARTUP) $(COMMON) ../../common/linker.ld
	$(CC) $(CFLAGS) $(SRCS) $(STARTUP) $(COMMON) $(LDFLAGS) -o main.elf

# Run in the emulator. Quit with Ctrl+A, release, then X.
run: main.elf
	qemu-system-arm -M lm3s6965evb -kernel main.elf -nographic

# Build and run the model solution instead of your own code
solution:
	$(MAKE) -B run SRCS="$(SOL_SRCS)" STARTUP="$(SOL_STARTUP)"

# Start the emulator paused and wait for a debugger on port 1234
debug: main.elf
	qemu-system-arm -M lm3s6965evb -kernel main.elf -nographic -S -gdb tcp::1234

# Inspection tools (all part of the ARM toolchain, see Lecture 7)
sections: main.elf
	arm-none-eabi-size -A main.elf

symbols: main.elf
	arm-none-eabi-nm -n main.elf

vectors: main.elf
	arm-none-eabi-objdump -s -j .isr_vector main.elf

disasm: main.elf
	arm-none-eabi-objdump -d -S main.elf

clean:
	rm -f main.elf main.map

.PHONY: run solution debug sections symbols vectors disasm clean
