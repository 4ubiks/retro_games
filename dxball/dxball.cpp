#include "dxballLibC.h"
#include "systemCalls.h"
#include "constants.h"

// aesthetic functions
void printWelcome();
void printDemo(int position, int positionMovement, char *buffer);
void printBackground();
void printGameboard();
void printBlocks(int numberOfBlocks, int startBlock, int xCoordinate, int color);
void printRules();
void printGameOver();
void printYouWin();

// game logic
void generateBall(int &ballX, int &ballY, int direction);
void printPlatform(int platformOffset, int platformArray[]);
int didHitPlatform(int platformArray[], int &ballX, int &ballDirection);

int main(){
	clearScreen();
	printWelcome();

	//int demoPosition=52;
	//int positionMovement=MOVE_RIGHT;
	char *buffer = (char *)KEYBOARD_BUFFER;
	
	while (true){
		
		readKeyboard(buffer);
		unsigned char testKey = (unsigned char)(*(char *)KEYBOARD_BUFFER);
			
		if (testKey != 0x93) {break;}
		else {printRules(); readKeyboard(buffer);}
	}

	int platformOffset=30;
	int ballX=34;
	int ballY=21;
	int ballMovement=BALL_UR;
	int platformArray[7] = {0};
	clearScreen();
	while (true){
	
		if (ballX == 70 && ballMovement == BALL_UR){
			ballMovement = BALL_UL;
		}
		else if (ballX == 70 && ballMovement == BALL_DR){
			ballMovement = BALL_DL;
		}
		else if (ballX == 6 && ballMovement == BALL_UL){
			ballMovement = BALL_UR;
		}
		else if (ballX == 6 && ballMovement == BALL_DL){
			ballMovement = BALL_DR;
		}
		if (ballY == 2 && ballMovement == BALL_UL){
			ballMovement = BALL_DL;
		}
		else if (ballY == 2 && ballMovement == BALL_UR){
			ballMovement = BALL_DR;
		}
		else if (ballY == 22 && ballMovement == BALL_DL){
			if (didHitPlatform(platformArray, ballX, ballMovement) == 1){
				ballMovement = BALL_UL;
			}
			else {printGameOver();}
		}
		else if (ballY == 22 && ballMovement == BALL_DR){
			if (didHitPlatform(platformArray, ballX, ballMovement) == 1){
				ballMovement = BALL_UR;
			}
			else {printGameOver();}
		}
		
		printGameboard();
		generateBall(ballX, ballY, ballMovement);
		char *bufferMemory = (char *)KEYBOARD_BUFFER;
		int keyPressed = checkKey((char *)bufferMemory);

		if (keyPressed){
			unsigned char directionKey = (unsigned char)*(char *)KEYBOARD_BUFFER;
			// 'a'
			if (directionKey == 0x1e && platformOffset > 8){
				platformOffset-=3;
				clearPlatform();
			}
			// 'd'
			else if (directionKey == 0x20 && platformOffset < 66){
				platformOffset+=3;
				clearPlatform();
			}
		}

		printPlatform(platformOffset, platformArray);
		delay(50000);
	}
}

void printWelcome() {
	printString((char *)" _______  ___   ___   ", 2, 5, COLOR_WHITE);
	printString((char *)"|       \\ \\  \\ /  /", 3, 5, COLOR_WHITE);
	printString((char *)"|  .--.  | \\  V  /   ", 4, 5, COLOR_WHITE);
	printString((char *)"|  |  |  |  >   <     ", 5, 5, COLOR_WHITE); 
	printString((char *)"|  '--'  | /  .  \\   ", 6, 5, COLOR_WHITE);
	printString((char *)"|_______/ /__/ \\__\\  ", 7, 5, COLOR_WHITE);   

	printString((char *)"||                 ", 3, 50, COLOR_WHITE);
	printString((char *)"||                 ", 4, 50, COLOR_WHITE);
	printString((char *)"||                 ", 5, 50, COLOR_WHITE);
	printString((char *)"  []", 3, 69, COLOR_GRAY);
	printString((char *)"[][]", 4, 69, COLOR_GRAY);
	printString((char *)"  []", 5, 69, COLOR_GRAY);
	
	printString((char *)"______  ______      _____       _____           _____", 8, 12, COLOR_YELLOW);
	printString((char *)"\\     \\|\\     \\   /      |_    |\\    \\         |\\    \\", 9, 12, COLOR_YELLOW);    
	printString((char *)" |     |\\|     | /         \\    \\\\    \\         \\\\    \\", 10, 12, COLOR_YELLOW);
	printString((char *)" |     |/____ / |     /\\    \\    \\\\    \\         \\\\    \\", 11, 12, COLOR_YELLOW);
	printString((char *)" |     |\\     \\ |    |  |    \\    \\|    | ______  \\|    | ______", 12, 12, COLOR_YELLOW);
	printString((char *)" |     | |     ||     \\/      \\    |    |/      \\  |    |/      \\", 13, 12, COLOR_YELLOW);
	printString((char *)" |     | |     ||\\      /\\     \\   /            |  /            |", 14, 12, COLOR_YELLOW);
	printString((char *)"/_____/|/_____/|| \\_____\\ \\_____\\ /_____/\\_____/| /_____/\\_____/|", 15, 12, COLOR_YELLOW);
	printString((char *)"|    |||     | || |     | |     ||      | |    |||      | |    ||", 16, 12, COLOR_YELLOW);
	printString((char *)"|____|/|_____|/  \\|_____|\\|_____||______|/|____|/|______|/|____|/", 17, 12, COLOR_YELLOW);  

	printString((char *)"Press any key to start!", 21, 26, COLOR_WHITE);
	printString((char *)"version 1.0", 23, 32, COLOR_DIRTY_BLUE);                           
}

void printDemo(int ballPosition, int positionMovement, char *buffer){
	while (ballPosition < 68){		
		printString((char *)" *", 4, ballPosition, COLOR_BALL_RED);
		ballPosition++;
		delay(50000);
	}
	positionMovement = MOVE_LEFT;
	while (ballPosition > 52){		
		ballPosition--;
		printString((char *)"* ", 4, ballPosition, COLOR_BALL_RED);
		delay(50000);
	}
}

void printBackground() {
	
}

void printGameboard() {
	printBlocks(9, 35, 4, COLOR_MAGENTA);
	printBlocks(10, 30, 5, COLOR_CYAN);
	printBlocks(11, 25, 6, COLOR_DIRTY_BLUE);
	printBlocks(12, 20, 7, COLOR_BLUE);
	printBlocks(13, 15, 8, COLOR_GREEN);
	printBlocks(14, 10, 9, COLOR_BALL_RED);
	printBlocks(15, 5, 10, COLOR_RED);
	printBlocks(14, 10, 11, COLOR_BALL_RED);
	printBlocks(13, 15, 12, COLOR_GREEN);
	printBlocks(12, 20, 13, COLOR_BLUE);
	printBlocks(11, 25, 14, COLOR_DIRTY_BLUE);
	printBlocks(10, 30, 15, COLOR_CYAN);
	printBlocks(9, 35, 16, COLOR_MAGENTA);
}

void printBlocks(int numberOfBlocks, int startBlock, int xCoordinate, int color){
	for (int i=startBlock; i<numberOfBlocks*5; i+=5){
		printString((char *)"[===]", xCoordinate, i, color);
	}
}

void printRules() {
	clearScreen();
	printString((char *)"there are rules.", 5, 15, COLOR_WHITE);
}

void printGameOver() {
	clearScreen();
	printString((char *)"game over...", 32, 15, COLOR_RED);	
	while (true){
		printString((char *)"game over...", 32, 15, COLOR_RED);		
	}
}

void printYouWin() {
	
}

// game logic
void generateBall(int &ballX, int &ballY, int ballDirection){

	if (ballDirection == BALL_UR){
		ballX+=2;
		ballY--;
		printString((char *)"{}", ballY, ballX, COLOR_YELLOW);
		printString((char *)"  ", ballY+1, ballX-2, 0x0);
	}
	else if (ballDirection == BALL_UL){
		ballX-=2;
		ballY--;
		printString((char *)"{}", ballY, ballX, COLOR_YELLOW);
		printString((char *)"  ", ballY+1, ballX+2, 0x0);
	}
	else if (ballDirection == BALL_DR){
		ballX+=2;
		ballY++;	
		printString((char *)"{}", ballY, ballX, COLOR_YELLOW);
		printString((char *)"  ", ballY-1, ballX-2, 0x0);
	}
	else if (ballDirection == BALL_DL){
		ballX-=2;
		ballY++;
		printString((char *)"{}", ballY, ballX, COLOR_YELLOW);
		printString((char *)"  ", ballY-1, ballX+2, 0x0);
	}

}

void printPlatform(int platformOffset, int platformArray[]){
	printString((char *)"<=======>", 22, platformOffset, COLOR_WHITE);
	for (int i=0; i<7; i++){
		platformArray[i] = platformOffset;
		platformOffset++;
	}
}

int didHitPlatform(int platformArray[], int &ballX, int &ballDirection){
	for (int i=0; i<7; i++){
		if (ballX == platformArray[i]){
			if (ballDirection == BALL_DL){
				ballDirection = BALL_UR;
			}
			else if (ballDirection == BALL_DR){
				ballDirection = BALL_UL;
			}
			return 1;
		}
	
	}
	return 0;
}
