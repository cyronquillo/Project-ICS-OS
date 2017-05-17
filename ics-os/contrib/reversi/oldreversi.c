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

int main(){

	initMatrix();
	startGame();
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
		while(1){
			int choice;
			int k = 0;
			do{
				matrix[possibleMoves[k][0]][possibleMoves[k][1]] = CURRENT;
				printMatrix(*(&matrix));
				matrix[possibleMoves[k][0]][possibleMoves[k][1]] = BLANK;
				// displayMoves(k);
				if(move == WHITE) printf("--- White Turn (1) ---\n");
				else printf("--- Black Turn (0) ---\n");
				printf("[a] Up\t[d] Down\t [Space] Drop\n");
				keypress=(char)getch();
				if(keypress == UP){
					k = k-1;
					if(k == -1) k = moveCounter-1;
				} 
				if(keypress == DOWN){
					k = (k+1) % moveCounter;
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
