ARMGNU ?= arm-none-eabi

CFLAGS = -Wall -nostdlib -fomit-frame-pointer -mno-apcs-frame -nostartfiles -ffreestanding -g -march=armv6z -marm -mthumb-interwork
ASFLAGS = -g -march=armv6z

C_FILES=kernel.c ../os/malloc.c ../os/hw.c ../os/sched.c ../os/sem.c
AS_FILES=vectors.s

OBJS = $(patsubst %.s,%.o,$(AS_FILES))
OBJS += $(patsubst %.c,%.o,$(C_FILES))

.PHONY: gcc clean

gcc : kernel

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f *.clang.opt.s

%.o : %.c
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

%.o : %.s
	$(ARMGNU)-as $(ASFLAGS) $< -o $@

kernel : ../os/memmap $(OBJS)
	$(ARMGNU)-ld $(OBJS) -T ../os/memmap -o kernel.elf
	$(ARMGNU)-objdump -D kernel.elf > kernel.list
	$(ARMGNU)-objcopy kernel.elf -O binary kernel.img
	$(ARMGNU)-objcopy kernel.elf -O ihex kernel.hex
