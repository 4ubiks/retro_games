#include "spaceLibC.h"
#include "constants.h"

void clearScreen(){
	char* vga_memory = (char *)0xb8000;

	for (int i=0; i<80*25; i++){
		vga_memory[i*2] = 0x0;
		vga_memory[i*2 + 1] = 0x0;
	}
}

void clearPlatform(){
	for (int i=0; i<80; i++){
		printString((char *)" ", 22, i, COLOR_WHITE);
	}
}

void printCharacter(char *message, int row, int column, int color)
{
    char *screen = (char *)0xb8000;

    int positionOffset = (row * 80 + column) * 2; // offset as we traverse through vga matrix

    // checks for special cases, but logic should be handled in printString()
    if (*message == 0x09) {column+=4; return;} 
    if (*message == 0x0A) {row+=1; column=0; return;}
    if (*message == 0x0D) {return;}

    // two bytes, one for message, one for display
    screen[positionOffset] = *message;
    screen[positionOffset + 1] = color;
}

void printString(char *message, int row, int column, int color)
{
    while (*message != 0x00){

        // logic to handle special cases
        while (*message == 0x0A || *message == 0x0D) {*message++; row++; column=0;}
        while (*message == 0x09){*message++; column+=4;}

        printCharacter(message, row, column, color);
        *message++;
        column++;
    }
}

void delay(unsigned long delayMS){
	delayMS *= 1000;
	while (delayMS--){
		__asm__ __volatile__("nop");
	}
}
