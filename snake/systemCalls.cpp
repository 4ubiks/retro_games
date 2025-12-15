#include "constants.h"
#include "systemCalls.h"

unsigned char inputIOPort(unsigned short port)
{
    unsigned char data;
    
    asm volatile("in %1,%0" : "=a" (data) : "d" (port));
    
    return data;
}

void readKeyboard(char* ioMemory)
{
    unsigned char scanCode;

    //status check
    while ((inputIOPort(KEYBOARD_STATUS_PORT) & 0x1) == 0) {}

    // I need to read twice to avoid duplicate scan codes
    inputIOPort(KEYBOARD_DATA_PORT); // Discarding first read

    //status check
    while ((inputIOPort(KEYBOARD_STATUS_PORT) & 0x1) == 0) {}

    scanCode = inputIOPort(KEYBOARD_DATA_PORT);
    *ioMemory = ((unsigned char)scanCode);
}

int checkKey(char* key){
	if (inputIOPort(KEYBOARD_STATUS_PORT) & 0x1){
		unsigned char scanCode;

		inputIOPort(KEYBOARD_DATA_PORT); // Discarding first read
		scanCode = inputIOPort(KEYBOARD_DATA_PORT);
		*key = ((unsigned char)scanCode);
		if (scanCode != 0){
			return 1;
		}
		return 0;
		
	}
	return 0;
}
