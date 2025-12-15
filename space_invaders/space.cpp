#include "spaceLibC.h"
#include "systemCalls.h"
#include "constants.h"

// cosmetic functions
void printWelcomeSpace();
void printWelcomeInvaders();
void printStartScreen();
void printCrab();
void printBug();
void printJellyfish();

int main(){
	clearScreen();
	while (true){
		printWelcomeSpace();
		delay(100000);
		clearScreen();
		delay(10000);
		printWelcomeInvaders();
		delay(100000);
		clearScreen();
		delay(10000);
		printCrab();
		delay(100000);
		clearScreen();
		
	}
}

void printWelcomeSpace(){
	printString((char *)"  ____  ____   _    ____ _____", 5, 12, COLOR_WHITE);
	printString((char *)" / ___||  _ \\ / \\  / ___| ____|", 6, 12, COLOR_WHITE);
	printString((char *)" \\___ \\| |_) / _ \\| |   |  _|", 7, 12, COLOR_WHITE);
	printString((char *)"  ___) |  __/ ___ \\ |___| |___", 8, 12, COLOR_WHITE);
	printString((char *)" |____/|_| /_/   \\_\\____|_____|", 9, 12, COLOR_WHITE);
}

void printWelcomeInvaders(){
	printString((char *)"  _____  _   _ __      __ ___      _____   ______  _____    _____", 12, 8, COLOR_WHITE);
	printString((char *)" |_   _|| \\ | |\\ \\    / //   \\    |  __ \\ |  ____||  __ \\  / ____|", 13, 8, COLOR_WHITE);
	printString((char *)"   | |  |  \\| | \\ \\  / //  ^  \\   | |  | || |__   | |__) || (___", 14, 8, COLOR_WHITE);
	printString((char *)"   | |  | . ` |  \\ \\/ //  /_\\  \\  | |  | ||  __|  |  _  /  \\___ \\", 15, 8, COLOR_WHITE);
	printString((char *)"  _| |_ | |\\  |   \\  //  _____  \\ | |__| || |____ | | \\ \\  ____) |", 16, 8, COLOR_WHITE);
	printString((char *)" |_____||_| \\_|    \\//__/     \\__\\|_____/ |______||_|  \\_\\|_____/", 17, 8, COLOR_WHITE);
}

void printStartScreen(){
	
}

void printCrab(){
	printString((char *)"  ", 2, 40, COLOR_BG_GREEN);
	printString((char *)"  ", 2, 54, COLOR_BG_GREEN);

	printString((char *)"  ", 3, 44, COLOR_BG_GREEN);
	printString((char *)"  ", 3, 50, COLOR_BG_GREEN);

	printString((char *)"                ", 4, 40, COLOR_BG_GREEN);
	printString((char *)"    ", 5, 38, COLOR_BG_GREEN);
	printString((char *)"        ", 5, 44, COLOR_BG_GREEN);
	printString((char *)"    ", 5, 54, COLOR_BG_GREEN);

	printString((char *)"                        ", 6, 36, COLOR_BG_GREEN);

	printString((char *)"  ", 7, 36, COLOR_BG_GREEN);
	printString((char *)"                ", 7, 40, COLOR_BG_GREEN);
	printString((char *)"  ", 7, 58, COLOR_BG_GREEN);
	
	printString((char *)"  ", 8, 36, COLOR_BG_GREEN);
	printString((char *)"  ", 8, 40, COLOR_BG_GREEN);
	printString((char *)"  ", 8, 54, COLOR_BG_GREEN);
	printString((char *)"  ", 8, 58, COLOR_BG_GREEN);

	printString((char *)"    ", 9, 42, COLOR_BG_GREEN);
	printString((char *)"    ", 9, 50, COLOR_BG_GREEN);
}

void printBug(){
	
}

void printJellyfish(){
	
}
