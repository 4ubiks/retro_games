#include "simpleOSlibc.h"
#include "constants.h"
#include "x86.h"
#include "screen.h"
#include "keyboard.h"
#include "vm.h"
#include "tetris_constants.h"

// templates
void welcomeScreen();
void rules();
void printBoard();
void printStats();
void printNext();
void printLogo();
void gameOver();
void printAll();

// game logic
void printPiece(int ROW_REDUCT, int COL_OFFSET, char piece[4][8]);
void printPieces(int pieceArray[20][20]);
void declarePiece(int pieceArrays[20][20], int my, int mx, int blockName);
int pickNewPiece(int currentBlock, int offset);
int checkTetris(int board[20][20]);

// prints user stuff
void printLines(int linesCleared);
void printLevel();
void printScore(int score);
void printTime(int currentTime);


// moving checks
int canMoveDown(int y, int x, int vbuffer, int board[20][20], int currentBlock);
int canMoveLeft(int y, int x, int board[20][20], int currentBlock);
int canMoveRight(int y, int x, int board[20][20], int currentBlock);

int isGameOver(int board[20][20]);

// graphics
void delay(int ms);
int checkKey(char* key); 

void main()
{   
	disableCursor();
	// have title page here
	char *buffer = (char *)KEYBOARD_BUFFER;

	while (true){
		welcomeScreen();
		readKeyboard(buffer);
		unsigned char testKey = (unsigned char)(*(char *)KEYBOARD_BUFFER);
		if (testKey != 0x93){break;}
		else {rules(); readKeyboard(buffer);}
	}

	// initializations

	// tracks piece location
	int ROW=0x1;
	int COL=0x0;

	// tracks coordinates for piece matrix
	int mx=0x0;
	int my=0x2;

	// tracks location
	int vbuffer = 0x0;
	int hbuffer = 0x0;

	// offset for picking next piece
	int offset=0x0;
	
	int tetrisRow=0x0;

	// this block contains all existing pieces
	int pieces[20][20] = {0x0};

	// user stats
	int linesCleared = 0x0;
	int score = 0x0;
	//int level = 0x1;
	int currentTime = 0x0;

	char linePiece[4][8] = {
		{'[', ']', ' ', ' ',' ', ' ', ' ', ' '},
		{'[', ']', ' ', ' ',' ', ' ', ' ', ' '},
		{'[', ']', ' ', ' ',' ', ' ', ' ', ' '},
		{'[', ']', ' ', ' ',' ', ' ', ' ', ' '}
	};

	char blockL[4][8] = {
		{'[', ']', ' ', ' ',' ', ' ', ' ', ' '},
		{'[', ']', ' ', ' ',' ', ' ', ' ', ' '},
		{'[', ']', '[', ']',' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ',' ', ' ', ' ', ' '}
	};

	char blockJ[4][8] = {
		{' ', ' ', '[', ']', ' ', ' ',' ', ' '},
		{' ', ' ', '[', ']', ' ', ' ',' ', ' '},
		{'[', ']', '[', ']',' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ',' ', ' ', ' ', ' '}
	};

	char square[4][8] = {
		{'[', ']', '[', ']',' ', ' ', ' ', ' '},
		{'[', ']', '[', ']',' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ',' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ',' ', ' ', ' ', ' '}
	};

	char blockS[4][8] = {
		{' ', ' ', '[', ']','[', ']', ' ', ' '},
		{'[', ']', '[', ']',' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ',' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ',' ', ' ', ' ', ' '}
	};

	char blockT[4][8] = {
		{' ', ' ', '[', ']',' ', ' ', ' ', ' '},
		{'[', ']', '[', ']','[', ']', ' ', ' '},
		{' ', ' ', ' ', ' ',' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ',' ', ' ', ' ', ' '}
	};

	char blockZ[4][8] = {
		{'[', ']', '[', ']',' ', ' ', ' ', ' '},
		{' ', ' ', '[', ']','[', ']', ' ', ' '},
		{' ', ' ', ' ', ' ',' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ',' ', ' ', ' ', ' '}
	};

	char current[4][8] = {' '};
	for (int row=0; row<4; row++){
		for (int col=0; col<8; col++){
			current[row][col] = linePiece[row][col];
		}
	}
	
	// for now, starting piece is always line. gotta be somewhat nice
	int currentBlock = LINE_PIECE;
	vbuffer = LINE_VBUFFER;
	hbuffer = LINE_HBUFFER;
	

	printAll();
	fillMemory((char *)KEYBOARD_BUFFER, (unsigned char)0x0, (KEYBOARD_BUFFER_SIZE * 2));

    while (true){
		char *bufferMem = (char *)KEYBOARD_BUFFER;
		
		int keyPressed = checkKey((char *)bufferMem);
		if (keyPressed){
			unsigned char key = (unsigned char)(*(char *)KEYBOARD_BUFFER);

			// press 'a'
			if (key == 0x9e){
				if (canMoveLeft(my, mx, pieces, currentBlock)){
					ROW--;
					COL-=2;
					my-=2;
				}
				printBoard();
				printPieces(pieces);

				// user values
				printLines(linesCleared);
				//printLevel();
				printScore(score);
				printTime(currentTime);

				printPiece(ROW, COL, current);
			}

			// press 'd'
			else if (key == 0xa0){
				if ((canMoveRight(my, mx, pieces, currentBlock))){
					ROW--;
					COL+=2;
					my+=2;
				}
				printBoard();
				printPieces(pieces);

				// user values
				printLines(linesCleared);
				//printLevel();
				printScore(score);
				printTime(currentTime);

				printPiece(ROW, COL, current);
			}

			// press 'r'
			else if (key == 0x93){
				// rotate piece, will be available in future versions
			}

			// press 'esc'
			else if (key == 0x81){
				systemSwitchToParent();
			}
		}
		delay(50000);
		
		printBoard();
		printPieces(pieces);

		// increment time
		currentTime++;

		// user values
		printLines(linesCleared);
		//printLevel();
		printScore(score);
		printTime(currentTime);

		printPiece(ROW, COL, current);
		
		if (canMoveDown(my, mx, vbuffer, pieces, currentBlock)){
			ROW++;
			mx = ROW;
		}
		else {	
			mx+=vbuffer;
			declarePiece(pieces, my, mx, currentBlock);

			// check to see if this completes a row
			tetrisRow = checkTetris(pieces);

			if (tetrisRow != -1){
				score += 10;
				linesCleared++;
				for (int col=0; col<20; col+=2){
					pieces[col][tetrisRow] = 0x0;
				}

				for (int row=19; row>=0; row--){
					if (row < tetrisRow){
						for (int col=0; col<20; col+=2){
							pieces[col][row+1] = pieces[col][row];
						}
					}
				}
			}
			else {
				score += 1;
			}
			printPieces(pieces);

			offset = mx*my;
			ROW=1;
			COL=0;
			mx=0x0;
			my=0x2;

			// switch to new block
			currentBlock = pickNewPiece(currentBlock, (offset+score));

			switch (currentBlock){
				case LINE_PIECE:
					for (int row=0; row<4; row++){
						for (int col=0; col<8; col++){
							current[row][col] = linePiece[row][col];
						}
					}
					vbuffer = LINE_VBUFFER;
					hbuffer = LINE_HBUFFER;
					break;
				
				case L_BLOCK:
					for (int row=0; row<4; row++){
						for (int col=0; col<8; col++){
							current[row][col] = blockL[row][col];
						}
					}
					vbuffer = LJ_VBUFFER;
					hbuffer = LJ_HBUFFER;
					break;

				case J_BLOCK:
					for (int row=0; row<4; row++){
						for (int col=0; col<8; col++){
							current[row][col] = blockJ[row][col];
						}
					}
					vbuffer = LJ_VBUFFER;
					hbuffer = LJ_HBUFFER;
					break;

				case SQUARE:
					for (int row=0; row<4; row++){
						for (int col=0; col<8; col++){
							current[row][col] = square[row][col];
						}
					}
					vbuffer = SQ_VBUFFER;
					hbuffer = SQ_HBUFFER;
					break;

				case S_BLOCK:
					for (int row=0; row<4; row++){
						for (int col=0; col<8; col++){
							current[row][col] = blockS[row][col];
						}
					}
					vbuffer = STZ_VBUFFER;
					hbuffer = STZ_HBUFFER;
					break;

				case T_BLOCK:
					for (int row=0; row<4; row++){
						for (int col=0; col<8; col++){
							current[row][col] = blockT[row][col];
						}
					}
					vbuffer = STZ_VBUFFER;
					hbuffer = STZ_HBUFFER;
					break;

				case Z_BLOCK:
					for (int row=0; row<4; row++){
						for (int col=0; col<8; col++){
							current[row][col] = blockZ[row][col];
						}
					}
					vbuffer = STZ_VBUFFER;
					hbuffer = STZ_HBUFFER;
					break;
			}

			if (isGameOver(pieces)) break;
			}
		}

	// game over code
	gameOver();
	wait(2);
	enableCursor();
	char *bufferMem = (char *)KEYBOARD_BUFFER;
	readKeyboard(bufferMem);
	unsigned char key = (unsigned char)(*(char *)KEYBOARD_BUFFER);
	if (key != 0x0){
		systemSwitchToParent();
	}
		
}

void welcomeScreen(){
	clearScreen();
	printString(COLOR_GREEN, 5, 5, (char *)"::::::::::: :::::::::: ::::::::::: :::::::::  :::::::::::  ::::::::  ");
	printString(COLOR_GREEN, 6, 5, (char *)"    :+:     :+:            :+:     :+:    :+:     :+:     :+:    :+:");
	printString(COLOR_GREEN, 7, 5, (char *)"    +:+     +:+            +:+     +:+    +:+     +:+     +:+       ");
	printString(COLOR_GREEN, 8, 5, (char *)"    +#+     +#++:++#       +#+     +#++:++#:      +#+     +#++:++#++");
	printString(COLOR_GREEN, 9, 5, (char *)"    +#+     +#+            +#+     +#+    +#+     +#+            +#+ ");
	printString(COLOR_GREEN, 10, 5, (char *)"    #+#     #+#            #+#     #+#    #+#     #+#     #+#    #+#");
	printString(COLOR_GREEN, 11, 5, (char *)"    ###     ##########     ###     ###    ### ###########  ########  ");

	printString(COLOR_WHITE, 19, 27, (char *)"Press any key to START...");
	printString(COLOR_WHITE, 21, 33, (char *)"'r' for rules");
	printString(COLOR_LIGHT_BLUE, 23, 34, (char *)"version 1.0");
}

void rules(){
	clearScreen();
	printString(COLOR_WHITE, 5, 5, (char *)"RULES:");
	printString(COLOR_WHITE, 7, 5, (char *)"Pieces will fall from above, you can move them left and right.");
	printString(COLOR_WHITE, 8, 5, (char *)"Getting a whole row of pieces will remove that row and earn points.");
	printString(COLOR_WHITE, 9, 5, (char *)"If a column fills to the top, you lose!");

	printString(COLOR_WHITE, 11, 5, (char *)"- 'a' to move LEFT");
	printString(COLOR_WHITE, 12, 5, (char *)"- 'd' to move RIGHT");
	printString(COLOR_WHITE, 13, 5, (char *)"- 'ESC' to exit the program");

	printString(COLOR_WHITE, 15, 5, (char *)"Your version of Tetris is on EXTREME difficulty!");
	printString(COLOR_WHITE, 16, 5, (char *)"This means you cannot rotate pieces!");
	printString(COLOR_WHITE, 17, 5, (char *)"However, the level (drop speed) will not increase.");

    printString(COLOR_WHITE, 19, 5, (char *)"Note: Do not spam keys. The system will stop receiving input.");

	printString(COLOR_WHITE, 22, 5, (char *)"Press any key to go back to start.");
}

void printBoard(){
	printString(COLOR_GREEN, 0, 30, (char *)"                        ");
    for (int row = 1; row < 21; row++){
		printString(COLOR_GREEN, row, 30, (char *)"<! . . . . . . . . . .!>");
	}
	printString(COLOR_GREEN, 21, 30, (char *)"<!********************!>");
	printString(COLOR_GREEN, 22, 30, (char *)"  \\/\\/\\/\\/\\/\\/\\/\\/\\/\\/ ");
}

void printStats(){
	printString(COLOR_GREEN, 0, 5, (char *)"*******************");
	for (int statRow=1; statRow < 7; statRow++){
		printString(COLOR_GREEN, statRow, 5, (char *)"*                 *");
	}
	printString(COLOR_GREEN, 2, 7, (char *)"FULL LINES:");
	printString(COLOR_GREEN, 3, 7, (char *)"LEVEL:");
	printString(COLOR_WHITE, 3, 21, (char *)"1");
	printString(COLOR_GREEN, 4, 7, (char *)"SCORE: ");
	printString(COLOR_GREEN, 5, 7, (char *)"TIME: ");

	printString(COLOR_GREEN, 6, 5, (char *)"*                 *");
	printString(COLOR_GREEN, 7, 5, (char *)"*******************");
}

void printNext(){
	printString(COLOR_GREEN, 0, 60, (char *)"<!*********!>");
	for (int nextRow=1; nextRow<6; nextRow++){
		printString(COLOR_GREEN, nextRow, 60, (char *)"<! . . . . !>");
	}
	printString(0x03, 6, 60, (char *)"<!*********!>");
}

void printLogo(){
	printString(COLOR_RED, 19, 0, (char *)"  _       _        _");
	printString(COLOR_RED, 20, 0, (char *)" | |     | |      (_)");
	printString(COLOR_RED, 21, 0, (char *)" | |_ ___| |_ _ __ _ ___");
	printString(COLOR_RED, 22, 0, (char *)" | __/ _ \\ __| '__| / __|");
	printString(COLOR_RED, 23, 0, (char *)" | ||  __/ |_| |  | \\__ \\");
	printString(COLOR_RED, 24, 0, (char *)"  \\__\\___|\\__|_|  |_|___/");
}

void gameOver(){
	clearScreen();
	printString(COLOR_RED, 2, 12, (char *)" ::::::::      :::     ::::    ::::  ::::::::::        ");
	printString(COLOR_RED, 3, 12, (char *)":+:    :+:   :+: :+:   +:+:+: :+:+:+ :+:              ");
	printString(COLOR_RED, 4, 12, (char *)"+:+         +:+   +:+  +:+ +:+:+ +:+ +:+              ");
	printString(COLOR_RED, 5, 12, (char *)":#:        +#++:++#++: +#+  +:+  +#+ +#++:++#         ");
	printString(COLOR_RED, 6, 12, (char *)"+#+   +#+# +#+     +#+ +#+       +#+ +#+              ");
	printString(COLOR_RED, 7, 12, (char *)"#+#    #+# #+#     #+# #+#       #+# #+#              ");
	printString(COLOR_RED, 8, 12, (char *)" ########  ###     ### ###       ### ##########        ");

	printString(COLOR_RED, 10, 12, (char *)" ::::::::  :::     ::: :::::::::: :::::::::        :::");
	printString(COLOR_RED, 11, 12, (char *)":+:    :+: :+:     :+: :+:        :+:    :+:       :+:");
	printString(COLOR_RED, 12, 12, (char *)"+:+    +:+ +:+     +:+ +:+        +:+    +:+       +:+");
	printString(COLOR_RED, 13, 12, (char *)"+#+    +:+ +#+     +:+ +#++:++#   +#++:++#:        +#+");
	printString(COLOR_RED, 14, 12, (char *)"+#+    +#+  +#+   +#+  +#+        +#+    +#+       +#+");
	printString(COLOR_RED, 15, 12, (char *)"#+#    #+#   #+#+#+#   #+#        #+#    #+#          ");
	printString(COLOR_RED, 16, 12, (char *)" ########      ###     ########## ###    ###       ###");

	printString(COLOR_WHITE, 22, 14, (char *)"Press any key to EXIT");
}

void printAll(){
	clearScreen();
    printBoard();
    printStats();
    printNext();
    printLogo();
}

void printPiece(int ROW_REDUCT, int COL_OFFSET, char piece[4][8]){
	for (int row=0; row<4; row++){
		for (int col=0; col<8; col++){
			if ((char)piece[row][col] != ' '){
				char temp[2];
				temp[0] = piece[row][col];
				temp[1] = '\0';
				printString(COLOR_RED, ROW_REDUCT+row, 34+COL_OFFSET+col, (char *)temp);
			}
		}
	}
}

void printPieces(int arr[20][20]){
	for (int i=0; i<20; i+=2){
		for (int j=0; j<20; j++){
			if (arr[i][j] == PIECE_PRESENT){
				printString(COLOR_RED, j+1, i+32, (char *)"[");
				printString(COLOR_RED, j+1, i+33, (char *)"]");				
			}
		}
	}
}

void delay(int ms){
	for (int i=0; i<ms*1000; i++){
		true == true;
	}
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

int checkTetris(int board[20][20]){
	int tetris=0x0;
	for (int row=0; row<20; row++){
		tetris=0x0;
		for (int col=0; col<20; col+=2){
			if (board[col][row] == 0){
				break;
			}
			else {tetris++;}
		}
		if (tetris == 10){
			return row;
		}
	}

	return -1;
}

// USER STUFF
void printLines(int linesCleared){
	char *buffer;
	buffer[0] = (linesCleared/100)%10+'0';
	buffer[1] = (linesCleared/10)%10+'0';
	buffer[2] = (linesCleared)%10+'0';
	buffer[3]='\0';

	printString(COLOR_WHITE, 2, 19, (char *)buffer);
}

void printLevel(){
	printString(COLOR_WHITE, 3, 18, (char *)"one");
}

void printScore(int score){
	char *buffer;
	buffer[0] = (score / 1000) % 10 + '0';
	buffer[1] = (score / 100) % 10 + '0';
	buffer[2] = (score / 10) % 10 + '0';
	buffer[3] = (score) % 10 + '0';
	buffer[4] = '\0';

	printString(COLOR_WHITE, 4, 18, (char *)buffer);
}

void printTime(int currentTime){
	int myTime=0x0;
	if (currentTime%5==0){
		myTime = currentTime/5;
	}

	char *buffer;
	buffer[0] = (myTime/100)%10+'0';
	buffer[1] = (myTime/10)%10+'0';
	buffer[2] = (myTime)%10+'0';
	buffer[3]='\0';

	if (currentTime%5==0) printString(COLOR_WHITE, 5, 19, (char *)buffer);
}


int canMoveDown(int y, int x, int vbuffer, int board[20][20], int currentBlock){
	// bottom
	if ((x+vbuffer) + 1 >= HEIGHT) return 0; 

	switch (currentBlock){
		case LINE_PIECE:
			// piece below      
    		if (board[y][(x+vbuffer)+1] == PIECE_PRESENT) return 0; 
			break;
		
		case L_BLOCK:
			if (board[y][(x+vbuffer)+1] == PIECE_PRESENT ||
				board[y+MOVE_RIGHT][(x+vbuffer)+1] == PIECE_PRESENT) 
				return 0;  
			break;

		case J_BLOCK:
			if (board[y][(x+vbuffer)+1] == PIECE_PRESENT ||
				board[y+MOVE_RIGHT][(x+vbuffer)+1] == PIECE_PRESENT) 
				return 0;  
			break;

		case SQUARE:
			if (board[y][(x+vbuffer)+1] == PIECE_PRESENT ||
				board[y+MOVE_RIGHT][(x+vbuffer)+1] == PIECE_PRESENT) 
				return 0;  
			break;

		case S_BLOCK:
			if (board[y][(x+vbuffer)+1] == PIECE_PRESENT ||
				board[y+MOVE_RIGHT][(x+vbuffer)+1] == PIECE_PRESENT ||
				board[y+(MOVE_RIGHT*2)][(x+vbuffer)] == PIECE_PRESENT) 
				return 0;  
			break;

		case T_BLOCK:
			if (board[y][(x+vbuffer)+1] == PIECE_PRESENT ||
				board[y+MOVE_RIGHT][(x+vbuffer)+1] == PIECE_PRESENT ||
				board[y+(MOVE_RIGHT*2)][(x+vbuffer)+1] == PIECE_PRESENT) 
				return 0; 
			break;

		case Z_BLOCK:
			if (board[y][(x+vbuffer)] == PIECE_PRESENT ||
				board[y+MOVE_RIGHT][(x+vbuffer)+1] == PIECE_PRESENT ||
				board[y+(MOVE_RIGHT*2)][(x+vbuffer+1)] == PIECE_PRESENT) 
				return 0; 
			break;
	}
    

	     
    return 1;
}

int canMoveLeft(int y, int x, int board[20][20], int currentBlock){
	// just check to see if you're against the wall
	if (y <= 0) return 0;

	switch (currentBlock){
		case LINE_PIECE:
			if (board[y-MOVE_LEFT][x] == PIECE_PRESENT ||
				board[y-MOVE_LEFT][x-1] == PIECE_PRESENT ||
				board[y-MOVE_LEFT][x-2] == PIECE_PRESENT ||
				board[y-MOVE_LEFT][x-3] == PIECE_PRESENT) 
				return 0;
			break;
		
		case L_BLOCK:
			if (board[y-MOVE_LEFT][x] == PIECE_PRESENT ||
				board[y-MOVE_LEFT][x-1] == PIECE_PRESENT ||
				board[y-MOVE_LEFT][x-2] == PIECE_PRESENT)
				return 0;
			break;

		case J_BLOCK:
			if (board[y-MOVE_LEFT][x] == PIECE_PRESENT ||
				board[y][x-1] == PIECE_PRESENT ||
				board[y][x-2] == PIECE_PRESENT)
				return 0;
			break;

		case SQUARE:
			if (board[y-MOVE_LEFT][x] == PIECE_PRESENT ||
				board[y-MOVE_LEFT][x-1] == PIECE_PRESENT)
				return 0;
			break;

		case S_BLOCK:
			if (board[y-MOVE_LEFT][x] == PIECE_PRESENT ||
				board[y][x-1] == PIECE_PRESENT)
				return 0;
			break;

		case T_BLOCK:
			if (board[y-MOVE_LEFT][x] == PIECE_PRESENT ||
				board[y][x-1] == PIECE_PRESENT)
				return 0;
			break;

		case Z_BLOCK:
			if (board[y-MOVE_LEFT][x-1] == PIECE_PRESENT ||
				board[y][x] == PIECE_PRESENT)
				return 0;
			break;
	}
	return 1;
}

int canMoveRight(int y, int x, int board[20][20], int currentBlock){
	switch (currentBlock){
		case LINE_PIECE:
			// line hits right wall
			if (y+MOVE_RIGHT >= WIDTH) return 0;

			// piece next to it 
			if (board[y+MOVE_RIGHT][x] == PIECE_PRESENT ||
				board[y+MOVE_RIGHT][x-1] == PIECE_PRESENT ||
				board[y+MOVE_RIGHT][x-2] == PIECE_PRESENT ||
				board[y+MOVE_RIGHT][x-3] == PIECE_PRESENT) 
				return 0;
			break;
		
		case L_BLOCK:
			// l hits right wall
			if (y+(MOVE_RIGHT*2) >= WIDTH) return 0;

			// piece check
			if (board[y+(MOVE_RIGHT*2)][x] == PIECE_PRESENT ||
				board[y+MOVE_RIGHT][x-1] == PIECE_PRESENT ||
				board[y+MOVE_RIGHT][x-2] == PIECE_PRESENT)
				return 0;
			break;

		case J_BLOCK:
			// j hits right wall
			if (y+(MOVE_RIGHT*2) >= WIDTH) return 0;

			// piece check
			if (board[y+(MOVE_RIGHT*2)][x] == PIECE_PRESENT ||
				board[y+(MOVE_RIGHT*2)][x-1] == PIECE_PRESENT ||
				board[y+(MOVE_RIGHT*2)][x-2] == PIECE_PRESENT)
				return 0;
			// code
			break;

		case SQUARE:
			// square hits right wall
			if (y+(MOVE_RIGHT*2) >= WIDTH) return 0;

			// piece check
			if (board[y+(MOVE_RIGHT*2)][x] == PIECE_PRESENT ||
				board[y+(MOVE_RIGHT*2)][x-1] == PIECE_PRESENT)
				return 0;
			break;

		case S_BLOCK:
			// s hits right wall
			if (y+(MOVE_RIGHT*3) >= WIDTH) return 0;

			// piece check
			if (board[y+(MOVE_RIGHT*2)][x] == PIECE_PRESENT ||
				board[y+(MOVE_RIGHT*3)][x-1] == PIECE_PRESENT)
				return 0;
			break;

		case T_BLOCK:
			// t hits right wall
			if (y+(MOVE_RIGHT*3) >= WIDTH) return 0;

			// piece check
			if (board[y+(MOVE_RIGHT*3)][x] == PIECE_PRESENT ||
				board[y+(MOVE_RIGHT*2)][x-1] == PIECE_PRESENT)
				return 0;
			break;

		case Z_BLOCK:
			// z hits right wall
			if (y+(MOVE_RIGHT*3) >= WIDTH) return 0;

			// piece check
			if (board[y+(MOVE_RIGHT*2)][x-1] == PIECE_PRESENT ||
				board[y+(MOVE_RIGHT*3)][x] == PIECE_PRESENT)
				return 0;
			break;
	}

	return 1;
}

void declarePiece(int pieces[20][20], int my, int mx, int blockName){
	switch (blockName){
		case LINE_PIECE:
			pieces[my][mx] = PIECE_PRESENT;
			pieces[my][mx-1] = PIECE_PRESENT;
			pieces[my][mx-2] = PIECE_PRESENT;
			pieces[my][mx-3] = PIECE_PRESENT;
			break;

		case L_BLOCK:
			pieces[my][mx] = PIECE_PRESENT;
			pieces[my+2][mx] = PIECE_PRESENT;
			pieces[my][mx-1] = PIECE_PRESENT;
			pieces[my][mx-2] = PIECE_PRESENT;

			break;

		case J_BLOCK:
			pieces[my][mx] = PIECE_PRESENT;
			pieces[my+2][mx] = PIECE_PRESENT;
			pieces[my+2][mx-1] = PIECE_PRESENT;
			pieces[my+2][mx-2] = PIECE_PRESENT;
			break;
		
		case SQUARE:
			pieces[my][mx] = PIECE_PRESENT;
			pieces[my][mx-1] = PIECE_PRESENT;
			pieces[my+2][mx] = PIECE_PRESENT;
			pieces[my+2][mx-1] = PIECE_PRESENT;
			break;
		
		case S_BLOCK:
			pieces[my][mx] = PIECE_PRESENT;
			pieces[my+2][mx] = PIECE_PRESENT;
			pieces[my+2][mx-1] = PIECE_PRESENT;
			pieces[my+4][mx-1] = PIECE_PRESENT;
			break;

		case T_BLOCK:
			pieces[my][mx] = PIECE_PRESENT;
			pieces[my+2][mx] = PIECE_PRESENT;
			pieces[my+2][mx-1] = PIECE_PRESENT;
			pieces[my+4][mx] = PIECE_PRESENT;
			break;
		
		case Z_BLOCK:
			pieces[my][mx-1] = PIECE_PRESENT;
			pieces[my+2][mx-1] = PIECE_PRESENT;
			pieces[my+2][mx] = PIECE_PRESENT;
			pieces[my+4][mx] = PIECE_PRESENT;
			break;
	}
}

int pickNewPiece(int currentPiece, int offset){
	int newPiece=0x1;

	newPiece *= offset;
	newPiece += 15;

	int retPiece = (newPiece % 7) + 1;

	return retPiece;
}

int isGameOver(int board[20][20]){
	for (int col=0; col<20; col+=2){
		if (board[col][0] == PIECE_PRESENT) return 1;
	}
	return 0;
}
