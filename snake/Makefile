default: build

build:
	nasm -f bin bootloader.asm
	gcc -ggdb -m32 -fno-pie -ffreestanding -fno-stack-protector -c snake.cpp -o snake.o -Wunused-variable
	gcc -ggdb -m32 -fno-pie -ffreestanding -fno-stack-protector -c snakeLibC.cpp -o snakeLibC.o -Wunused-variable
	gcc -ggdb -m32 -fno-pie -ffreestanding -fno-stack-protector -c systemCalls.cpp -o systemCalls.o -Wunused-variable
	
	ld -m elf_i386 -e main -Ttext 0x9000 snakeLibC.o systemCalls.o snake.o -o snake

	dd if=/dev/zero of=snake.img bs=1M count=2
	cat bootloader | dd of=snake.img bs=1 seek=0 conv=notrunc
	cat snake | dd of=snake.img bs=1 seek=512 conv=notrunc

	@echo -e "\e[32mLAUNCHING SNAKE...\e[97m"
	cp snake.img ../games/.
	qemu-system-i386 -drive file=snake.img,format=raw -monitor stdio

run:
	qemu-system-i386 -drive file=snake.img,format=raw -monitor stdio

clean:
	rm -f *.o
	rm -f bootloader
	rm -f snake
	rm -f snake.img
