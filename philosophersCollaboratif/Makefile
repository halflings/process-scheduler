ARMGNU ?= arm-none-eabi

CFLAGS = -Wall -nostdlib -fomit-frame-pointer -mno-apcs-frame -nostartfiles -ffreestanding -g -march=armv6z -marm -mthumb-interwork
ASFLAGS = -g -march=armv6z

OS_DIR = ../os/

C_FILES=philosophers.c $(OS_DIR)malloc.c $(OS_DIR)hw.c $(OS_DIR)sched.c $(OS_DIR)sem.c $(OS_DIR)dispatcher.c
AS_FILES=vectors.s

OBJS = $(patsubst %.s,%.o,$(AS_FILES))
OBJS += $(patsubst %.c,%.o,$(C_FILES))

.PHONY: gcc clean

gcc : kernel run

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

kernel : $(OS_DIR)memmap $(OBJS)
	$(ARMGNU)-ld $(OBJS) -T $(OS_DIR)memmap -o kernel.elf
	$(ARMGNU)-objdump -D kernel.elf > kernel.list
	$(ARMGNU)-objcopy kernel.elf -O binary kernel.img
	$(ARMGNU)-objcopy kernel.elf -O ihex kernel.hex
	
run :
	cd debug; gnome-terminal -x ./run-qemu.sh; ./launch-gdb.sh;
