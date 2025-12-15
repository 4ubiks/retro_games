#include "snakeLibC.h"
#include "systemCalls.h"
#include "constants.h"

// aesthetic functions
void printWelcome();
void printRules();
void printBlankBoard();
void gameOver();
void youWin();

// game logic
typedef struct {int x; int y;} Pair;
void drawSnake(int snakeOriginX, int snakeOriginY, Pair snakePositions[], int sizeOfSnake);

void moveDirection(int currentDirection, Pair snakePositions[], int sizeOfSnake);
void moveSnakeRight(Pair snakePositions[], int sizeOfSnake);
void moveSnakeLeft(Pair snakePositions[], int sizeOfSnake);
void moveSnakeUp(Pair snakePositions[], int sizeOfSnake);
void moveSnakeDown(Pair snakePositions[], int sizeOfSnake);
int checkOuterBounds(Pair snakePositions[]);
int checkHittingYourself(Pair snakePositions[], int sizeOfSnake);
int checkIfWon(Pair snakePositions[]);

Pair generateNextApplePosition(Pair snakePositions[], int x, int y);
void checkIfApple(Pair snakePositions[], Pair applesToInitialize[], int &sizeOfSnake, int currentDirection, int &appleScore);
void printApples(Pair applesToInitialize[]);
void printAppleScore(int appleCount);

int main(){
	clearScreen();

	char *buffer = (char *)KEYBOARD_BUFFER;
	
	while (true){
		printWelcome();
		readKeyboard(buffer);
		unsigned char testKey = (unsigned char)(*(char *)KEYBOARD_BUFFER);
		if (testKey != 0x93){break;}
		else {printRules(); readKeyboard(buffer);}
	}

	int snakeOriginX=44;
	int snakeOriginY=12;

	// initializing snake
	Pair snakePositions[400] = {0};
	snakePositions[0] = (Pair){44, 12};
	snakePositions[1] = (Pair){44, 13};
	snakePositions[2] = (Pair){44, 14};
	snakePositions[3] = (Pair){46, 14};
	snakePositions[4] = (Pair){48, 14};

	// initializing apples
	Pair applesToEat[5] = {0};
	applesToEat[0] = (Pair){24, 17};
	applesToEat[1] = (Pair){54, 5};
	applesToEat[2] = (Pair){10, 12};
	applesToEat[3] = (Pair){38, 10};
	applesToEat[4] = (Pair){12, 11};

	int sizeOfSnake=5;
	int currentDirection=MOVE_UP;
	int appleCounter=0;

	clearScreen();
	printBlankBoard();
	while (true){
		printBlankBoard();
		drawSnake(snakeOriginX, snakeOriginY, snakePositions, sizeOfSnake);
		printApples(applesToEat);
		printAppleScore(appleCounter);

		char *bufferMemory = (char *)KEYBOARD_BUFFER;
		int keyPressed = checkKey((char *)bufferMemory);

		moveDirection(currentDirection, snakePositions, sizeOfSnake);

		checkIfApple(snakePositions, applesToEat, sizeOfSnake, currentDirection, appleCounter);
		if (checkHittingYourself(snakePositions, sizeOfSnake) == 1){
			delay(200000);
			gameOver();
		};
		
		if (checkOuterBounds(snakePositions) == 1){
			delay(500000);
			gameOver();
		}

		if (checkIfWon(snakePositions) == 1){
			delay(200000);
			youWin();
		}
		
		if (keyPressed){
			unsigned char key = (unsigned char)(*(char *)KEYBOARD_BUFFER);

			// 'a' (0x9e break code)
			if (key == 0x1e && currentDirection != MOVE_RIGHT) {
				currentDirection = MOVE_LEFT;
			}

			// 'd' (0xa0 break code)
			else if (key == 0x20 && currentDirection != MOVE_LEFT) {
				currentDirection = MOVE_RIGHT;
			}

			// 'w' (0x91 break code)
			else if (key == 0x11 && currentDirection != MOVE_DOWN) {
				currentDirection = MOVE_UP;
			}

			// 's' (0x9f break code)
			else if (key == 0x1f && currentDirection != MOVE_UP) {
				currentDirection = MOVE_DOWN;
			}
		}

		delay(50000);
	}
}

// PRINT FUNCTIONS
void printWelcome(){
	clearScreen();
	printString((char *)" ::::::::  ::::    :::     :::     :::    ::: :::::::::: ", 5, 10, COLOR_GREEN);
	printString((char *)":+:    :+: :+:+:   :+:   :+: :+:   :+:   :+:  :+:        ", 6, 10, COLOR_GREEN);
	printString((char *)"+:+        :+:+:+  +:+  +:+   +:+  +:+  +:+   +:+        ", 7, 10, COLOR_GREEN);
	printString((char *)"+#++:++#++ +#+ +:+ +#+ +#++:++#++: +#++:++    +#++:++#   ", 8, 10, COLOR_GREEN);
	printString((char *)"       +#+ +#+  +#+#+# +#+     +#+ +#+  +#+   +#+        ", 9, 10, COLOR_GREEN);
	printString((char *)"#+#    #+# #+#   #+#+# #+#     #+# #+#   #+#  #+#        ", 10, 10, COLOR_GREEN);
	printString((char *)" ########  ###    #### ###     ### ###    ### ########## ", 11, 10, COLOR_GREEN);
	printString((char *)"Press any key to continue...", 15, 26, COLOR_WHITE);

	printString((char *)"version 1.0", 20, 32, COLOR_DIRTY_BLUE);
}

void printRules(){
	clearScreen();
	// RULES:
	// You are a snake. To grow large enough to leave your garden,
	// you must eat enough apples to grow until you can take up
	// the whole garden! 
	// Watch out, because if you run into yourself, you'll fall
	// asleep, and you'll need to restart!

	// Controls:
	// W, A, S, D movements

	// Press any key to return to the homescreen.
	printString((char *)"RULES", 2, 30, COLOR_WHITE);
	printString((char *)"You are a snake. To grow large enough to leave your garden,", 4, 5, COLOR_WHITE);
	printString((char *)"you must eat enough apples to grow until you can take up", 5, 5, COLOR_WHITE);
	printString((char *)"the whole garden!", 6, 5, COLOR_WHITE);
	printString((char *)"Watch out, because if you run into yourself, you'll fall", 7, 5, COLOR_WHITE);
	printString((char *)"asleep, and you'll need to restart!", 8, 5, COLOR_WHITE);
	printString((char *)"Controls:", 10, 5, COLOR_WHITE);
	printString((char *)"W, A, S, D movements", 11, 5, COLOR_WHITE);
	printString((char *)"Press any key to return to the homescreen.", 15, 15, COLOR_WHITE); }

void printBlankBoard(){
	for (int y=2; y<22; y++){
		for (int x=8; x<70; x+=2){
			printString((char *)"[]", y, x, COLOR_GREEN);
		}
	}
}

void gameOver(){
	clearScreen();
	printString((char *)"game over...", 12, 35, COLOR_RED);
	while (true){
		1==1;
	}
}

void youWin(){
	printString((char *)"you win!", 12, 36, COLOR_GREEN);
}

void drawSnake(int snakeOriginX, int snakeOriginY, Pair snakePositions[], int sizeOfSnake){

	int counter=0;
	while (snakePositions[counter].x != 0x0 && snakePositions[counter].y != 0x0){
		printString((char *)"[]", snakePositions[counter].y, snakePositions[counter].x, COLOR_RED);
		counter++;
	}
}

void moveDirection(int direction, Pair snakePositions[], int sizeOfSnake) {
	if (direction == MOVE_LEFT){
		moveSnakeLeft(snakePositions, sizeOfSnake);
	}

	else if (direction == MOVE_RIGHT){
		moveSnakeRight(snakePositions, sizeOfSnake);
	}

	else if (direction == MOVE_UP){
		moveSnakeUp(snakePositions, sizeOfSnake);
	}

	else if (direction == MOVE_DOWN){
		moveSnakeDown(snakePositions, sizeOfSnake);
	}
}

void moveSnakeRight(Pair snakePositions[], int sizeOfSnake){

	int newX=snakePositions[0].x + 2;
	int newY=snakePositions[0].y;

	for (int counter=sizeOfSnake; counter>0; counter--){
		snakePositions[counter] = (Pair){snakePositions[counter-1].x, snakePositions[counter-1].y};
	}
	
	snakePositions[0] = {newX, newY};
}

void moveSnakeLeft(Pair snakePositions[], int sizeOfSnake){

	int newX=snakePositions[0].x - 2;
	int newY=snakePositions[0].y;

	for (int counter=sizeOfSnake; counter>0; counter--){
		snakePositions[counter] = (Pair){snakePositions[counter-1].x, snakePositions[counter-1].y};
	}
	
	snakePositions[0] = {newX, newY};
}

void moveSnakeUp(Pair snakePositions[], int sizeOfSnake){

	int newX=snakePositions[0].x;
	int newY=snakePositions[0].y-1;

	for (int counter=sizeOfSnake; counter>0; counter--){
		snakePositions[counter] = (Pair){snakePositions[counter-1].x, snakePositions[counter-1].y};
	}
	
	snakePositions[0] = {newX, newY};
}

void moveSnakeDown(Pair snakePositions[], int sizeOfSnake){

	int newX=snakePositions[0].x;
	int newY=snakePositions[0].y+1;

	for (int counter=sizeOfSnake; counter>0; counter--){
		snakePositions[counter] = (Pair){snakePositions[counter-1].x, snakePositions[counter-1].y};
	}
	
	snakePositions[0] = {newX, newY};
}

int checkIfWon(Pair snakePositions[]) {
	for (int snakeInc=0; snakeInc<0x190; snakeInc++){
		if (snakePositions[snakeInc].x == 0x0 && snakePositions[snakeInc].y == 0x0){
			return 0;
		}
	}
	return 1;
}

// GAME LOGIC
Pair generateNextApplePosition(Pair snakePositions[], int x, int y){
	unsigned int seed = 12345;

	seed = snakePositions[3].x * snakePositions[2].y + seed;

	int newAppleX = (seed*2 % 46) + 20;
	int newAppleY = ((seed*x + y) % 18) + 2;

	return {newAppleX, newAppleY};
	
}

void checkIfApple(Pair snakePositions[], Pair applesToInitialize[], int &sizeOfSnake, int currentDirection, int &appleScore){
	for (int appleChecks=0; appleChecks<NUMBER_OF_APPLES; appleChecks++){
		if (applesToInitialize[appleChecks].x == snakePositions[0].x && applesToInitialize[appleChecks].y == snakePositions[0].y){
//			printString((char *)"you ate an ample", 1, 1, COLOR_WHITE);

			if (currentDirection == MOVE_LEFT || currentDirection == MOVE_RIGHT){
				snakePositions[sizeOfSnake+1] = {snakePositions[sizeOfSnake].x+1, snakePositions[sizeOfSnake].y};
			}

			else {
				snakePositions[sizeOfSnake+1] = {snakePositions[sizeOfSnake].x, snakePositions[sizeOfSnake].y-1};
			}

			sizeOfSnake++;

			applesToInitialize[appleChecks] = generateNextApplePosition(snakePositions, snakePositions[sizeOfSnake].x, snakePositions[sizeOfSnake].y);

			appleScore++;
			
		}
	}
}


void printApples(Pair applesToInitialize[]){
	for (int apple=0; apple<5; apple++){
		printString((char *)"{}", applesToInitialize[apple].y, applesToInitialize[apple].x, COLOR_APPLE_RED);
	}
}

void printAppleScore(int appleCount){
	char* buffer;
	buffer[0] = (appleCount/100)%10 + '0';
	buffer[1] = (appleCount/10)%10 + '0';
	buffer[2] = appleCount%10 + '0';
	buffer[3] = '\0';

	printString((char *)buffer, 2, 2, COLOR_WHITE);
}

int checkOuterBounds(Pair snakePositions[]){
	if (snakePositions[0].x == 6 || snakePositions[0].x == 70){
		return 1;
	}
	else if (snakePositions[0].y == 1 || snakePositions[0].y == 22){
		return 1;
	}
	else{
		return 0;
	}
}

int checkHittingYourself(Pair snakePositions[], int sizeOfSnake){
	for (int currentSnakePosition=1; currentSnakePosition<sizeOfSnake; currentSnakePosition++){
		if (snakePositions[currentSnakePosition].x == snakePositions[0].x && snakePositions[currentSnakePosition].y == snakePositions[0].y){
			return 1;
		}
	}
	return 0;
}
