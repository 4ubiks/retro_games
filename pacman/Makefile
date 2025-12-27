default: build run

build:
	nasm -f bin bootloader.asm
	gcc -ggdb -m32 -fno-pie -ffreestanding -fno-stack-protector -c pacman.cpp -o pacman.o -Wunused-variable
	gcc -ggdb -m32 -fno-pie -ffreestanding -fno-stack-protector -c pacmanLibC.cpp -o pacmanLibC.o -Wunused-variable
	gcc -ggdb -m32 -fno-pie -ffreestanding -fno-stack-protector -c systemCalls.cpp -o systemCalls.o -Wunused-variable
	
	ld -m elf_i386 -e main -Ttext 0x9000 pacmanLibC.o systemCalls.o pacman.o -o pacman

	dd if=/dev/zero of=pacman.img bs=1M count=2
	cat bootloader | dd of=pacman.img bs=1 seek=0 conv=notrunc
	cat pacman | dd of=pacman.img bs=1 seek=512 conv=notrunc

	@echo -e "\e[32mLAUNCHING PACMAN...\e[97m"
	@cp pacman.img ../games/.

run:
	qemu-system-i386 -drive file=pacman.img,format=raw -monitor stdio

clean:
	rm -f *.o
	rm -f bootloader
	rm -f pacman
	rm -f pacman.img
