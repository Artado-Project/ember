GPP=i686-elf-g++
AS=nasm
GCC=i686-elf-gcc
CFLAGS=-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -I./include
QEMU=qemu-system-i386
SOURCES=kernel.cpp lib/*.cpp
QEMUFLAGS=-drive file=Arus.iso,format=raw,index=0,media=disk -serial file:/dev/stdout

build:
	for file in $(SOURCES); do \
		$(GPP) -c $$file -o `basename $$file .cpp`.o $(CFLAGS) ; \
	done
	$(AS) -felf32 boot.asm -o boot.o
	$(GCC) -T link.ld -o iso/boot/arus.bin -ffreestanding -O2 -nostdlib *.o -lgcc
	grub-file --is-x86-multiboot iso/boot/arus.bin
	grub-mkrescue -o Arus.iso iso

run:
	@$(QEMU) $(QEMUFLAGS)

clean:
	rm -f *.o Arus.iso iso/boot/arus.bin
