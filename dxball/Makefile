default: build run

build:
	nasm -f bin bootloader.asm
	gcc -ggdb -m32 -fno-pie -ffreestanding -fno-stack-protector -c dxball.cpp -o dxball.o -Wunused-variable
	gcc -ggdb -m32 -fno-pie -ffreestanding -fno-stack-protector -c dxballLibC.cpp -o dxballLibC.o -Wunused-variable
	gcc -ggdb -m32 -fno-pie -ffreestanding -fno-stack-protector -c systemCalls.cpp -o systemCalls.o -Wunused-variable
	
	ld -m elf_i386 -e main -Ttext 0x9000 dxballLibC.o systemCalls.o dxball.o -o dxball

	dd if=/dev/zero of=dxball.img bs=1M count=2
	cat bootloader | dd of=dxball.img bs=1 seek=0 conv=notrunc
	cat dxball | dd of=dxball.img bs=1 seek=512 conv=notrunc

	@echo -e "\e[32mLAUNCHING DXBALL...\e[97m"
	@cp dxball.img ../games/.

run:
	qemu-system-i386 -drive file=dxball.img,format=raw -monitor stdio

clean:
	rm -f *.o
	rm -f bootloader
	rm -f dxball
	rm -f dxball.img
