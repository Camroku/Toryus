GPP=i686-elf-g++
AS=nasm
GCC=i686-elf-gcc
CFLAGS=-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -I./include
QEMU=qemu-system-i386
SOURCES=kernel.cpp lib/*.cpp
QEMUFLAGS=-drive file=Toryus.iso,format=raw,index=0,media=disk -serial file:/dev/stdout

build:
	for file in $(SOURCES); do \
		$(GPP) -c $$file -o `basename $$file .cpp`.o $(CFLAGS) ; \
	done
	$(AS) -felf32 boot.asm -o boot.o
	$(GCC) -T link.ld -o iso/boot/toryus.bin -ffreestanding -O2 -nostdlib *.o -lgcc
	grub-file --is-x86-multiboot iso/boot/toryus.bin
	grub-mkrescue -o Toryus.iso iso

run:
	@$(QEMU) $(QEMUFLAGS)

clean:
	rm -f *.o Toryus.iso iso/boot/toryus.bin
