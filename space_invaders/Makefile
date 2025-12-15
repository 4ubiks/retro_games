default: build

build:
	nasm -f bin bootloader.asm
	gcc -ggdb -m32 -fno-pie -ffreestanding -fno-stack-protector -c space.cpp -o space.o -Wunused-variable
	gcc -ggdb -m32 -fno-pie -ffreestanding -fno-stack-protector -c spaceLibC.cpp -o spaceLibC.o -Wunused-variable
	gcc -ggdb -m32 -fno-pie -ffreestanding -fno-stack-protector -c systemCalls.cpp -o systemCalls.o -Wunused-variable
	
	ld -m elf_i386 -e main -Ttext 0x9000 spaceLibC.o systemCalls.o space.o -o space

	dd if=/dev/zero of=space.img bs=1M count=2
	cat bootloader | dd of=space.img bs=1 seek=0 conv=notrunc
	cat space | dd of=space.img bs=1 seek=512 conv=notrunc

	@echo -e "\e[32mLAUNCHING SPACE INVADERS...\e[97m"
	@cp space.img ../games/.
	qemu-system-i386 -drive file=space.img,format=raw -monitor stdio

run:
	qemu-system-i386 -drive file=space.img,format=raw -monitor stdio

clean:
	rm -f *.o
	rm -f bootloader
	rm -f space
	rm -f space.img
