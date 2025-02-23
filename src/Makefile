AS=nasm
GCC=i686-elf-gcc
INCLUDE=./include
CFLAGS=-ffreestanding -O2 -Wall -Wextra -fno-exceptions -I$(INCLUDE)
QEMU=qemu-system-i386
OBJDIR=./build
SOURCES=kernel.c $(wildcard lib/*/*.c)
OBJS = $(SOURCES:%.c=$(OBJDIR)/%.c.o) $(OBJDIR)/boot.o
QEMUFLAGS=-drive file=Toryus.iso,format=raw,index=0,media=disk -serial file:/dev/stdout

build:
	mkdir -p $(OBJDIR)
ifndef TORYUS_DEBUG
	for file in $(SOURCES); do \
		mkdir -p $(OBJDIR)/$$(dirname $$file); \
		$(GCC) -c $$file -o $(OBJDIR)/$$file.o $(CFLAGS) ; \
	done
else
	for file in $(SOURCES); do \
		mkdir -p $(OBJDIR)/$$(dirname $$file); \
		$(GCC) -c $$file -o $(OBJDIR)/$$file.o $(CFLAGS) ; \
	done
endif
	$(AS) -felf32 boot.asm -o $(OBJDIR)/boot.o
	$(GCC) -T link.ld -o iso/boot/toryus.bin -ffreestanding -O2 -nostdlib $(OBJS) -lgcc
	grub-file --is-x86-multiboot iso/boot/toryus.bin
	grub-mkrescue -o Toryus.iso iso

run:
	@$(QEMU) $(QEMUFLAGS)

clean:
	rm -rf Toryus.iso iso/boot/toryus.bin build/
