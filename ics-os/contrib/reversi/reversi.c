#include "../../sdk/dexsdk.h"
#include "../../sdk/time.h"

#define WHITE '1'
#define BLACK '0'
#define BLANK '-'
#define CURRENT '*'
#define UP 'a'
#define DOWN 'd'
#define DROP ' '
#define TRUE 1
#define FALSE 0
#define CHECK 0
#define PERFORM 1

//---------------------------
#define START_GAME 's'
#define EXIT_GAME 'e'
#define WHITE_COLOR 63
#define BLUE_COLOR 1
#define BLACK_COLOR 0
#define GRAY_COLOR 56
#define RED_COLOR 4
#define GREEN_COLOR 2
#define BROWN_COLOR 6
#define LIGHT_GREEN_COLOR 10
#define MAGENTA_COLOR 5
#define CYAN_COLOR 3
//---------------------------

char matrix[8][8];
int possibleMoves[64][2];
int moveCounter;
void printMatrix();
void initMatrix();
void startGame();
int isValid(int row, int col);
int listPossibleMoves(char color);
int checkAdjacents(int row, int col, int rowInc, int colInc, char color, int rowOrig, int colOrig, int action);
int matrixIsNotFull();
void displayMoves(int k);
int boardCleared();
int checkWinner();

//-----------------------------
void erase();
void printInitialBoard(char move);
void printPiece(int x, int y, int color);
void erasePiece(int x, int y);
void printBoard(char move);
//-----------------------------

int main(){

	char keypress = START_GAME;

	set_graphics(VGA_320X200X256);
	//set_coordinates(X_coord, Y_coord);
	

	initMatrix();
	do{
		erase(0,0,320,200);
		printMenu();
		keypress=(char)getch();
		erase(0,0,320,200);

		if(keypress == START_GAME){
			startGame();
		}
	}while(keypress != EXIT_GAME);

	set_graphics(VGA_TEXT80X25X16);
	clrscr();
}

void startGame(){
	char keypress;
	int i, j, row = 0, column = 0, turnCounter = 0;
	char move;
	int possible;
	while(matrixIsNotFull()){
		move = turnCounter %2 == 0? WHITE: BLACK;
		if(listPossibleMoves(move) == 0){
			turnCounter++;
			continue;
		}
		printInitialBoard(move);
		while(1){
			int choice;
			int k = 0;
			do{
				matrix[possibleMoves[k][0]][possibleMoves[k][1]] = CURRENT;
				printBoard(move);				
				matrix[possibleMoves[k][0]][possibleMoves[k][1]] = BLANK;
				//printInitialBoard(move);
				keypress=(char)getch();
				if(keypress == UP){
					k = k-1;
					if(k == -1) k = moveCounter-1;
				} 
				if(keypress == DOWN){
					k = (k+1) % moveCounter;
				}
				if(keypress == EXIT_GAME){
					return;
				}
			} while(keypress!=DROP);
			
			row = possibleMoves[k][0];
			column = possibleMoves[k][1];
			if(row < 0 || row > 7 || column  < 0 || column > 7) {
				printf("Invalid Input! \n");
				continue;
			}
			else if(matrix[row][column] != BLANK){
				printf("Space already occupied!\n");
				continue;
			}
		
			if (checkAdjacents(row-1, column-1, -1, -1,move , row, column, PERFORM)) possible = 1;
			if (checkAdjacents(row-1, column  , -1,  0,move , row, column, PERFORM)) possible = 1;
			if (checkAdjacents(row-1, column+1, -1,  1,move , row, column, PERFORM)) possible = 1;
			if (checkAdjacents(row  , column-1,  0, -1,move , row, column, PERFORM)) possible = 1;
			if (checkAdjacents(row  , column+1,  0,  1,move , row, column, PERFORM)) possible = 1;
			if (checkAdjacents(row+1, column-1,  1, -1,move , row, column, PERFORM)) possible = 1;
			if (checkAdjacents(row+1, column  ,  1,  0,move , row, column, PERFORM)) possible = 1;
			if (checkAdjacents(row+1, column+1,  1,  1,move , row, column, PERFORM)) possible = 1;
			if(possible == 1){
				matrix[row][column] = move;
				turnCounter++;
				break;
			} else{
				printf("No flipping occurred!\n");
			}
			printBoard(move);
		}

	}
	checkWinner();
}


int checkWinner(){
	int black = 0;
	int white = 0;
	int i,j;
	for(i = 0; i < 8; i++){
		for(j = 0; j < 8; j++){
			black += (matrix[i][j] == BLACK)?1:0;
			white += (matrix[i][j] == WHITE)?1:0;
		}
	}
	printf("BLACK: %d WHITE: %d\n",black,white);
	if(black > white) 
		printf("BLACK WINS!\n");
	else if(white > black) 
		printf("WHITE WINS!\n");
	else 
		printf("DRAW!\n");
}
int listPossibleMoves(char color){
	moveCounter = 0;
	int row, column;
	for(row = 0; row < 8; row++){
		for(column = 0; column < 8; column++){
			if(matrix[row][column] != BLANK) continue;
			if ( 	checkAdjacents(row-1, column-1, -1, -1,color , row, column, CHECK) ||
					checkAdjacents(row-1, column  , -1,  0,color , row, column, CHECK) ||
					checkAdjacents(row-1, column+1, -1,  1,color , row, column, CHECK) ||
					checkAdjacents(row  , column-1,  0, -1,color , row, column, CHECK) ||
					checkAdjacents(row  , column+1,  0,  1,color , row, column, CHECK) ||
					checkAdjacents(row+1, column-1,  1, -1,color , row, column, CHECK) ||
					checkAdjacents(row+1, column  ,  1,  0,color , row, column, CHECK) ||
					checkAdjacents(row+1, column+1,  1,  1,color , row, column, CHECK)){
				moveCounter +=1;
				// if(moveCounter == 1) printf("List of Possible Moves: \n");
				possibleMoves[moveCounter-1][0] = row;
				possibleMoves[moveCounter-1][1] = column;
			}
		}
	}

	return moveCounter;
}

void displayMoves(int k){
	int i = 0;
	for(i = 0; i < moveCounter; i++){
		printf("(%d, %d)",possibleMoves[i][0]+1,possibleMoves[i][1]+1);
		if(i == k) printf(" <");
		printf("\n");
	}
}

int checkAdjacents(int row, int col, int rowInc, int colInc, char color, int rowOrig, int colOrig, int action){
	if(row < 0 || row > 7 || col  < 0 || col > 7) return 0;
	if(matrix[row][col] == color && (rowOrig + rowInc == row) && (colOrig + colInc == col)) return 0;
	if(matrix[row][col] == BLANK) return 0; 
	if(matrix[row][col] == color){
		// change all values :D
		if(action == PERFORM){
			while(row != rowOrig || col != colOrig){
				matrix[row][col] = color;
				row -= rowInc;
				col -= colInc;
			}
		}
		return 1;
	}
	checkAdjacents(row +rowInc, col + colInc, rowInc, colInc, color, rowOrig, colOrig, action);
}
void printMatrix(){
	int i, j;
	printf("\n\n\t");
	printf("\n\t");
	for(i = 1; i < 9; i++) printf("%d\t", i);
	printf("\n\n");
	for(i = 0; i < 8; i++){
		printf("%d\t", i+1);
		for(j = 0; j < 8; j++){
			printf("%c\t", matrix[i][j]);
		}
		printf("\n\n");

	}
}

int matrixIsNotFull(){
	int i, j;

	for(i = 0; i < 8; i++){
		for(j = 0; j < 8; j++){
			if(matrix[i][j] == BLANK){
				return 1;
			}
		}
	}

	return 0;
}

void initMatrix(){
	int i,j;
	for(i = 0; i < 8; i++){
		for(j = 0; j < 8; j++){
			matrix[i][j] = BLANK;
		}
	}
	matrix[4][4] = matrix[3][3] = WHITE;
	matrix[4][3] = matrix[3][4] = BLACK;
}

//-----------------------------------------------

void erase(int x, int y, int w, int h){
   int i,j;
   for (i=y;i<=(y+h);i++)
      for (j=x;j<=(x+w);j++)
         write_pixel(j,i,100);
}

printMenu(){
	int i;
	for(i=0;i<317;i+=6){
		write_pixel(i,0,WHITE_COLOR);
		write_pixel(i+1,0,WHITE_COLOR);
		write_pixel(i+2,0,WHITE_COLOR);
	}
	for(i=3;i<318;i+=6){
		write_pixel(i,0,BLUE_COLOR);
		write_pixel(i+1,0,BLUE_COLOR);
		write_pixel(i+2,0,BLUE_COLOR);
	}
	write_text("---REVERSI---", 105,40,WHITE_COLOR,1);

	write_text("S - Start", 120,100,WHITE_COLOR,0);
	write_text("E - Exit", 120, 130,WHITE_COLOR,0);
}

void printInitialBoard(char move){
	int i,j;
	erase(5,30,50,10);
	if(move == WHITE){
		write_text("WHITE TURN", 5,30,WHITE_COLOR,0);
	}
	else{
		write_text("BLACK TURN", 5,30,WHITE_COLOR,0);
	}
	write_text("A-Up", 5,45,WHITE_COLOR,0);
	write_text("D-Down", 5,55,WHITE_COLOR,0);
	write_text("Space-Drop", 5,65,WHITE_COLOR,0);

	for(i=5;i<196;i++){
		for(j=114;j<305;j++){
			write_pixel(j,i,LIGHT_GREEN_COLOR);
		}
	}
	for(i=4;i<197;i++){ //vertical lines
		write_pixel(113,i,WHITE_COLOR);
		write_pixel(137,i,WHITE_COLOR);
		write_pixel(161,i,WHITE_COLOR);
		write_pixel(185,i,WHITE_COLOR);
		write_pixel(209,i,WHITE_COLOR);
		write_pixel(233,i,WHITE_COLOR);
		write_pixel(257,i,WHITE_COLOR);
		write_pixel(281,i,WHITE_COLOR);
		write_pixel(305,i,WHITE_COLOR);
	}
	for(i=114;i<305;i++){ //horizontal lines
		write_pixel(i,4,WHITE_COLOR);
		write_pixel(i,28,WHITE_COLOR);
		write_pixel(i,52,WHITE_COLOR);
		write_pixel(i,76,WHITE_COLOR);
		write_pixel(i,100,WHITE_COLOR);
		write_pixel(i,124,WHITE_COLOR);
		write_pixel(i,148,WHITE_COLOR);
		write_pixel(i,172,WHITE_COLOR);
		write_pixel(i,196,WHITE_COLOR);
	}
	printBoard(move);
}

void printBoard(char move){
	int i,j;

	if(move == WHITE){
		write_text("WHITE TURN", 5,30,WHITE_COLOR,0);
	}
	else{
		write_text("BLACK TURN", 5,30,WHITE_COLOR,0);
	}

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			if(matrix[i][j] == WHITE){
				printPiece(i,j,WHITE);
			}
			else if(matrix[i][j] == BLACK){
				printPiece(i,j,BLACK);
			}
			else if(matrix[i][j] == CURRENT){
				printPiece(i,j,GRAY_COLOR);
			}
			else if(matrix[i][j] == BLANK){
				erasePiece(i,j);
			}
		}
	}
}

void printPiece(int x, int y, int color){

	//int bordercolor = color == WHITE? BLACK_COLOR: WHITE_COLOR;

	int i, j, bordercolor;

	switch(color){
		case WHITE: bordercolor = WHITE_COLOR;break;
		case BLACK: bordercolor = BLACK_COLOR;break;
		case GRAY_COLOR: bordercolor = GRAY_COLOR;break;
	}

	int verticalBoundPairStart = 120 + (x*24) - 1;
	int verticalBoundPairEnd = verticalBoundPairStart + 12;
	int yPosVerticalTopBound = 9 + (y*24) - 1;
	int yPosVerticalDownBound = yPosVerticalTopBound + 16;

	for(i=verticalBoundPairStart;i<=verticalBoundPairEnd;i++){
		write_pixel(i,yPosVerticalTopBound,bordercolor);
		write_pixel(i,yPosVerticalDownBound,bordercolor);
	}
	write_pixel(verticalBoundPairStart-1,yPosVerticalTopBound+1, bordercolor);
	write_pixel(verticalBoundPairStart,yPosVerticalTopBound+1, bordercolor);
	write_pixel(verticalBoundPairEnd,yPosVerticalTopBound+1, bordercolor);
	write_pixel(verticalBoundPairEnd+1,yPosVerticalTopBound+1, bordercolor);

	write_pixel(verticalBoundPairStart-2,yPosVerticalTopBound+2, bordercolor);
	write_pixel(verticalBoundPairStart-1,yPosVerticalTopBound+2, bordercolor);
	write_pixel(verticalBoundPairEnd+1,yPosVerticalTopBound+2, bordercolor);
	write_pixel(verticalBoundPairEnd+2,yPosVerticalTopBound+2, bordercolor);

	write_pixel(verticalBoundPairStart-3,yPosVerticalTopBound+3, bordercolor);
	write_pixel(verticalBoundPairStart-2,yPosVerticalTopBound+3, bordercolor);
	write_pixel(verticalBoundPairEnd+2,yPosVerticalTopBound+3, bordercolor);
	write_pixel(verticalBoundPairEnd+3,yPosVerticalTopBound+3, bordercolor);

	for(i=4;i<14;i++){
		write_pixel(verticalBoundPairStart-3,yPosVerticalTopBound+i, bordercolor);
		write_pixel(verticalBoundPairEnd+3,yPosVerticalTopBound+i, bordercolor);
	}

	write_pixel(verticalBoundPairStart-1,yPosVerticalDownBound-1, bordercolor);
	write_pixel(verticalBoundPairStart,yPosVerticalDownBound-1, bordercolor);
	write_pixel(verticalBoundPairEnd,yPosVerticalDownBound-1, bordercolor);
	write_pixel(verticalBoundPairEnd+1,yPosVerticalDownBound-1, bordercolor);

	write_pixel(verticalBoundPairStart-2,yPosVerticalDownBound-2, bordercolor);
	write_pixel(verticalBoundPairStart-1,yPosVerticalDownBound-2, bordercolor);
	write_pixel(verticalBoundPairEnd+1,yPosVerticalDownBound-2, bordercolor);
	write_pixel(verticalBoundPairEnd+2,yPosVerticalDownBound-2, bordercolor);

	write_pixel(verticalBoundPairStart-3,yPosVerticalDownBound-3, bordercolor);
	write_pixel(verticalBoundPairStart-2,yPosVerticalDownBound-3, bordercolor);
	write_pixel(verticalBoundPairEnd+2,yPosVerticalDownBound-3, bordercolor);
	write_pixel(verticalBoundPairEnd+3,yPosVerticalDownBound-3, bordercolor);
}

void erasePiece(int x, int y){
	int i, j;
	int startXErase = 114 + (x*24);
	int endXErase = startXErase + 23;
	int startYErase = 5 + (y*24);
	int endYErase = startYErase + 23;
	erase(startXErase, startYErase,22,22);

	for(i=startXErase;i<endXErase;i++){
		for(j=startYErase;j<endYErase;j++){
			write_pixel(i,j,LIGHT_GREEN_COLOR);
		}
	}
}