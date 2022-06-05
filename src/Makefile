build:
	i686-elf-g++ -c kernel.cpp -o kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
	nasm -felf32 boot.asm -o boot.o
	i686-elf-gcc -T link.ld -o iso/boot/arus.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc
	grub-file --is-x86-multiboot iso/boot/arus.bin
	grub-mkrescue -o Arus.iso iso

run:
	qemu-system-i386 Arus.iso

clean:
	rm -f kernel.o boot.o Arus.iso iso/boot/arus.bin
