ARMGNU ?= arm-none-eabi

OBJDUMP = $(ARMGNU)-objdump
LD = $(ARMGNU)-ld
OBJCOPY = $(ARMGNU)-objcopy
CC = $(ARMGNU)-gcc
AS = $(ARMGNU)-as

CFLAGS = -Wall -nostdlib -fomit-frame-pointer -mno-apcs-frame -nostartfiles -ffreestanding -g -march=armv6z -marm
ASFLAGS = -g -march=armv6z

C_FILES=hw.c allocateMemory.c sched.c sem.c

OBJS += $(patsubst %.c,%.o, $(C_FILES))

.PHONY: gcc clean

gcc: $(OBJS)

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f *.clang.opt.s
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
