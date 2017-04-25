
#include <stdio.h>
#define WHITE 1
#define BLACK -1

int matrix[8][8];

void printMatrix(int matrix[8][8]);
void initMatrix();
void startGame();
int isValid(int row, int col);
int matrixIsNotFull(int matrix[8][8]);

int main(){
	initMatrix();	
	startGame();
}

void startGame(){
	int i, j, row = 0, column = 0, turnCounter = 0;
	int move;
	while(matrixIsNotFull(*(&matrix))){
		printMatrix(*(&matrix));
		move = turnCounter++ %2 == 0? WHITE: BLACK;
		if(move == WHITE) printf("---White Turn---\n");
		else printf("---Black Turn---\n");

		printf("Enter Row[1-8]: ");
		scanf("%d", &row);
		row -= 1;
		printf("Enter Column[1-8]: ");
		scanf("%d", &column);
		column -= 1;

		if(turnCounter++ % 2 == 0){
			matrix[row-1][column-1] = WHITE;
		}
		else{
			matrix[row-1][column-1] = BLACK;
		}
	}
}

void printMatrix(int matrix[8][8]){
	int i, j;

	printf("\n");
	
	for(i = 0; i < 8; i++){
		for(j = 0; j < 8; j++){
			printf("%4d\t", matrix[i][j]);
		}
		printf("\n\n");

	}
}

int matrixIsNotFull(int matrix[8][8]){
	int i, j;

	for(i = 0; i < 8; i++){
		for(j = 0; j < 8; j++){
			if(matrix[i][j] == 0){
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
			matrix[i][j] = 0;
		}
	}
	matrix[4][4] = matrix[3][3] = WHITE;
	matrix[4][3] = matrix[3][4] = BLACK;
}