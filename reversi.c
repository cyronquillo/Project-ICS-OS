
#include <stdio.h>
#define WHITE '1'
#define BLACK '0'
#define BLANK '-'
#define TRUE 1
#define FALSE 0
#define CHECK 0
#define PERFORM 1

char matrix[8][8];

void printMatrix();
void initMatrix();
void startGame();
int isValid(int row, int col);
int listPossibleMoves(char color);
int checkAdjacents(int row, int col, int rowInc, int colInc, char color, int rowOrig, int colOrig, int action);
int matrixIsNotFull();

int main(){
	initMatrix();	
	startGame();
}

void startGame(){
	int i, j, row = 0, column = 0, turnCounter = 0;
	char move;
	int possible;
	while(matrixIsNotFull()){
		printMatrix(*(&matrix));
		move = turnCounter %2 == 0? WHITE: BLACK;

		if(move == WHITE) printf("--- White Turn (1) ---\n");
		else printf("--- Black Turn (0) ---\n");
		if(listPossibleMoves(move) == 0){
			turnCounter++;
			continue;
		}
		while(1){
			possible = 0;
			printf("Enter Row[1-8]: ");
			scanf("%d", &row);
			row -= 1;
			printf("Enter Column[1-8]: ");
			scanf("%d", &column);
			column -= 1;
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
}


int listPossibleMoves(char color){
	int counter = 0;
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
				counter +=1;
				if(counter == 1) printf("List of Possible Moves: \n");
				printf("(%d, %d)\n",row+1,column+1);
			}
		}
	}

	return counter;
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