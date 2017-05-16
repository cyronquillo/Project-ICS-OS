#include "../../sdk/dexsdk.h"
#include "../../sdk/time.h"

//--------------------------
/*backend constants*/
#define WHITE '1'
#define BLACK '0'	
#define BLANK '-'   // no piece on the board yet
#define CURRENT '*' // chosen position but not yet dropped down
#define DROP ' '	// space bar to drop a piece
#define CHECK 0
#define PERFORM 1
//---------------------------
/*front end constants*/
#define START_GAME 's'
#define EXIT_GAME 'e'
#define UP 'a'		
#define DOWN 'd'	
#define RESET 'r'	
#define WHITE_COLOR 63
#define BLUE_COLOR 1
#define BLACK_COLOR 0
#define GRAY_COLOR 56
#define RED_COLOR 4
#define GREEN_COLOR 2
#define BROWN_COLOR 6
#define LIGHT_BLUE_COLOR 9
#define LIGHT_GREEN_COLOR 10
#define MAGENTA_COLOR 5
#define CYAN_COLOR 3
//---------------------------
/*global variables*/
char matrix[8][8];
int possibleMoves[64][2];
int moveCounter;
int blackCounter;
int whiteCounter;
char keypress;
//----------------------------
/*backend function prototypes*/
void initMatrix();
void startGame();
int listPossibleMoves(char color);
int checkAdjacents(int row, int col, int rowInc, int colInc, char color, int rowOrig, int colOrig, int action);
int matrixIsNotFull();
int checkWinner();

//-----------------------------
/*frontend function prototypes*/
void erase();
void printInitialBoard();
void printPiece(int x, int y, int color);
void erasePiece(int x, int y);
void printBoard(char move);
void printMenu();
void set_graphics(int type);
void write_pixel(int x, int y, int color);
void write_text(char text[], int x, int y, int color, int size);
void sprintf(char text[], char type[], int color);

//-----------------------------

int main(){

	keypress = START_GAME;

	set_graphics(VGA_320X200X256);
	//set_coordinates(X_coord, Y_coord);
	

	do{
		initMatrix();					// calls initMatrix to reset all values in the board
		erase(0,0,320,200);
		printMenu();
		keypress=(char)getch();
		erase(0,0,320,200);

		if(keypress == START_GAME){ //starts the game
			while(1){
				initMatrix();
				startGame();
				if(keypress == EXIT_GAME){
					keypress = START_GAME;
					break;
				}
			}
		}
		
	}while(keypress != EXIT_GAME);

	set_graphics(VGA_TEXT80X25X16);
	clrscr();
}

void startGame(){
	int i, j, row = 0, column = 0, turnCounter = 0,temp;
	char move;
	printInitialBoard();
	while(matrixIsNotFull()){
		move = turnCounter %2 == 0? WHITE: BLACK;
		if(listPossibleMoves(move) == 0){ 
			//checks if the current player has a valid move
			if(listPossibleMoves((turnCounter+1)%2) == 0) break;
				// checks if the next player has a valid move as well
			turnCounter++;
			continue; 
			// if current player has no valid move, the turn is given to the other player
			// if both players have no valid move, the game is ended and a winner is determined
		}
		while(1){
			printBoard(move);
			int choice;
			int k = 0;
			do{
				matrix[possibleMoves[k][0]][possibleMoves[k][1]] = CURRENT;
				printPiece(possibleMoves[k][0],possibleMoves[k][1],GRAY_COLOR);
				// switches the space's color to gray when it is the chosen position
				matrix[possibleMoves[k][0]][possibleMoves[k][1]] = BLANK;
				keypress=(char)getch();
				if(keypress == UP){ //traverses to the list of possible moves
					erasePiece(possibleMoves[k][0],possibleMoves[k][1]);
					k = k-1;
					if(k == -1) k = moveCounter-1;
				} 
				if(keypress == DOWN){ // traverses to the list of possible moves
					erasePiece(possibleMoves[k][0],possibleMoves[k][1]);
					k = (k+1) % moveCounter;
				}
				if(keypress == EXIT_GAME || keypress == RESET){ // goes back to main menu 
					return;
				}
			} while(keypress!=DROP);
			// gets the chosen move on the list of possible moves
			row = possibleMoves[k][0]; 
			column = possibleMoves[k][1];
			printPiece(row,column,move); 
			// temp is just a holder for the return value.
			// it is not important in this function 
			temp = checkAdjacents(row-1, column-1, -1, -1,move , row, column, PERFORM);
			temp = checkAdjacents(row-1, column  , -1,  0,move , row, column, PERFORM);
			temp = checkAdjacents(row-1, column+1, -1,  1,move , row, column, PERFORM);
			temp = checkAdjacents(row  , column-1,  0, -1,move , row, column, PERFORM);
			temp = checkAdjacents(row  , column+1,  0,  1,move , row, column, PERFORM);
			temp = checkAdjacents(row+1, column-1,  1, -1,move , row, column, PERFORM);
			temp = checkAdjacents(row+1, column  ,  1,  0,move , row, column, PERFORM);
			temp = checkAdjacents(row+1, column+1,  1,  1,move , row, column, PERFORM);
			matrix[row][column] = move; 
			turnCounter++; // changes the turn
			break;
			
		}

	}
	printBoard(move); 					
	checkWinner(); 
	/*calls this function after the board is filled or 
	there are no more possible moves for both parties*/
}


int checkWinner(){
	char blackCount[5];
	char whiteCount[5];
	int black = 0;
	int white = 0;
	int i,j;
	// counts the black and white pieces to determine the winner
	for(i = 0; i < 8; i++){
		for(j = 0; j < 8; j++){
			black += (matrix[i][j] == BLACK)?1:0; 
			white += (matrix[i][j] == WHITE)?1:0;
		}
	}
	erase(5,30,105,200);
	write_text("Black: ", 5,30,WHITE_COLOR,0);
	sprintf(blackCount,"%d", black);
	write_text(blackCount, 80,30,WHITE_COLOR,0);
	write_text("White: ", 5, 50,WHITE_COLOR,0);
	sprintf(whiteCount,"%d", white);
	write_text(whiteCount, 80,50,WHITE_COLOR,0);
	if(black > white)
		write_text("Black Wins!", 10, 80,WHITE_COLOR,1); 
	else if(white > black) 
		write_text("White Wins!", 10, 80,WHITE_COLOR,1); 
	else
		write_text("Draw", 10, 80,WHITE_COLOR,1);  
	write_text("press any", 5, 170,WHITE_COLOR,0);
	write_text("key to ", 5, 180,WHITE_COLOR,0);
	write_text("exit...", 5, 190,WHITE_COLOR,0);
	keypress=(char)getch();
	keypress = EXIT_GAME;
}
int listPossibleMoves(char color){ 
// creates a list of possible moves by the current user by using checkAdjacents to all pieces in the board
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
				possibleMoves[moveCounter-1][0] = row;
				possibleMoves[moveCounter-1][1] = column;
			}
		}
	}

	return moveCounter;
}

int count(){ 	// counts the number of black pieces and white pieces in the current board displayed
	blackCounter = 0;
	whiteCounter = 0;
	int i,j;
	for(i = 0; i < 8; i++){
		for(j = 0; j < 8;j++){
			if(matrix[i][j] == BLACK)blackCounter++;
			if(matrix[i][j] == WHITE)whiteCounter++;
		}
	}
}
/*
checkAdjacent is a recursive function wherein a certain coordinate is given
and checks whether that coordinate has valid moves and can change a piece's
color once dropped down.

first if statement - checks if the current piece is already out of bounds or
					 not within the board anymore
second if statement -checks if the piece adjacent to it is the same with the
					 chosen color's pieces. If so, it will not extend in 
					 that portion
third if statement - checks if the current position is blank
fourth if statement - checks if the current position is the same color as the
					  chosen piece. If so, all pieces between the current 
					  position and the chosen position will change according
					  to the color of the piece in the chosen position
recursive call - 1st param and 2nd param: branches out from the original piece
				 3rd param and 4th param: increments to the row and column
				 						  depending on what condition to check
				 5th param: color of the chosen piece
				 6th param and 7th param: original value of the 1st and 2nd param
				 						  in the first recursive iteration
				 8th param: can be PERFORM or CHECK- CHECK: to see if it is a valid move
				 									 PERFORM: to see if it is a valid move,
				 									 		  and if so, performs flipping
				 									 		  of pieces

*/
int checkAdjacents(int row, int col, int rowInc, int colInc, char color, int rowOrig, int colOrig, int action){
	if(row < 0 || row > 7 || col  < 0 || col > 7) return 0;
	if(matrix[row][col] == color && (rowOrig + rowInc == row) && (colOrig + colInc == col)) return 0;
	if(matrix[row][col] == BLANK) return 0; 
	if(matrix[row][col] == color){
		// change all values :D
		if(action == PERFORM){
			while(row != rowOrig || col != colOrig){
				matrix[row][col] = color;
				printPiece(row,col,color);
				row -= rowInc;
				col -= colInc;
			}
		}
		return 1;
	}
	checkAdjacents(row +rowInc, col + colInc, rowInc, colInc, color, rowOrig, colOrig, action);
}


int matrixIsNotFull(){ // checks if there are still blanks on the board that can be filled with pieces
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
			matrix[i][j] = BLANK; // all the pieces in the board are removed
		}
	}
	matrix[4][4] = matrix[3][3] = WHITE;	// places 2 white pieces on coordinates (4,4) and (3,3)
	matrix[4][3] = matrix[3][4] = BLACK;	// places 2 white pieces on coordinates (4,4) and (3,3)
}

//-----------------------------------------------

void erase(int x, int y, int w, int h){
   int i,j;
   for (i=y;i<=(y+h);i++)
      for (j=x;j<=(x+w);j++)
         write_pixel(j,i,100);
}

void printMenu(){
	int i,j,temp;
	for(i=0;i<320;i++){
		for(j=0;j<200;j++){
			write_pixel(i,j,LIGHT_GREEN_COLOR);
		}
	}

	temp=0;
	for(i=40;i<=85;i++){
		for(j=0;j<7;j++){
			write_pixel(temp,i,WHITE_COLOR);
			temp++;
		}
	}
	write_pixel(0,86,LIGHT_GREEN_COLOR);
	write_pixel(1,86,LIGHT_GREEN_COLOR);

	for(i=10;i<310;i++){
		write_pixel(i,10,GRAY_COLOR);
		write_pixel(i,11,GRAY_COLOR);
		write_pixel(i,188,GRAY_COLOR);
		write_pixel(i,189,GRAY_COLOR);
	}

	for(i=10;i<190;i++){
		write_pixel(10,i,GRAY_COLOR);
		write_pixel(11,i,GRAY_COLOR);
		write_pixel(308,i,GRAY_COLOR);
		write_pixel(309,i,GRAY_COLOR);
	}

	for(i=125;i<195;i++){
		write_pixel(i,55,BLACK_COLOR);
		write_pixel(i,54,BLACK_COLOR);
		write_pixel(i,79,BLACK_COLOR);
		write_pixel(i,80,BLACK_COLOR);
		write_pixel(i,56,LIGHT_BLUE_COLOR);
		write_pixel(i,57,LIGHT_BLUE_COLOR);
		write_pixel(i,77,LIGHT_BLUE_COLOR);
		write_pixel(i,78,LIGHT_BLUE_COLOR);
	}

	write_pixel(124,56,BLACK_COLOR);
	write_pixel(123,56,BLACK_COLOR);
	write_pixel(195,56,BLACK_COLOR);
	write_pixel(196,56,BLACK_COLOR);
	write_pixel(124,57,BLACK_COLOR);
	write_pixel(123,57,BLACK_COLOR);
	write_pixel(195,57,BLACK_COLOR);
	write_pixel(196,57,BLACK_COLOR);
	write_pixel(124,78,BLACK_COLOR);
	write_pixel(123,78,BLACK_COLOR);
	write_pixel(195,78,BLACK_COLOR);
	write_pixel(196,78,BLACK_COLOR);
	write_pixel(124,77,BLACK_COLOR);
	write_pixel(123,77,BLACK_COLOR);
	write_pixel(195,77,BLACK_COLOR);
	write_pixel(196,77,BLACK_COLOR);

	for(i=58;i<77;i++){
		write_pixel(121,i,BLACK_COLOR);
		write_pixel(122,i,BLACK_COLOR);
		write_pixel(197,i,BLACK_COLOR);
		write_pixel(198,i,BLACK_COLOR);
	}

	for(i=58;i<77;i++){
		for(j=123;j<197;j++){
			write_pixel(j,i,LIGHT_BLUE_COLOR);
		}
	}


	write_text("REVERSI", 130,60,BLACK_COLOR,1);

	write_text("S-Start", 130,100,BLACK_COLOR,0);
	write_text("E-Exit", 130, 130,BLACK_COLOR,0);
}

void printInitialBoard(){
	int i,j;
	/*erase(5,30,50,10);
	if(move == WHITE){
		write_text("WHITE TURN", 5,30,WHITE_COLOR,0);
	}
	else{
		write_text("BLACK TURN", 5,30,WHITE_COLOR,0);
	}*/
	write_text("A-Up", 5,45,WHITE_COLOR,0);
	write_text("D-Down", 5,55,WHITE_COLOR,0);
	write_text("Space-Drop", 5,65,WHITE_COLOR,0);
	write_text("R-Reset Game", 5,180,WHITE_COLOR,0);
	write_text("E-Exit", 5,190,WHITE_COLOR,0);

	for(i=5;i<196;i++){
		for(j=114;j<305;j++){
			write_pixel(j,i,LIGHT_GREEN_COLOR);
		}
	}
	for(i=4;i<197;i++){ //vertical liness
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

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			if(matrix[i][j] == WHITE){
				printPiece(i,j,WHITE);
			}
			else if(matrix[i][j] == BLACK){
				printPiece(i,j,BLACK);
			}
		}
	}
}

void printBoard(char move){
	int i,j;

	char blackcounter[5];
	char whitecounter[5];

	erase(5,30,50,10);
	erase(5,95,100,20);

	if(move == WHITE){
		write_text("WHITE TURN", 5,30,WHITE_COLOR,0);
	}
	else{
		write_text("BLACK TURN", 5,30,WHITE_COLOR,0);
	}

	count();
	sprintf(blackcounter,"%d",blackCounter);
	sprintf(whitecounter,"%d",whiteCounter);

	write_text("Black: ", 5, 95, WHITE_COLOR,0);
	write_text(blackcounter, 80, 95, WHITE_COLOR,0);

	write_text("White: ", 5, 105, WHITE_COLOR,0);
	write_text(whitecounter, 80, 105, WHITE_COLOR,0);

}

void printPiece(int x, int y, int color){

	//int bordercolor = color == WHITE? BLACK_COLOR: WHITE_COLOR;

	int i, j, bordercolor, fillcolor;

	switch(color){
		case WHITE: bordercolor = BLACK_COLOR;fillcolor = WHITE_COLOR;break;
		case BLACK: bordercolor = WHITE_COLOR;fillcolor = BLACK_COLOR;break;
		case GRAY_COLOR: bordercolor = GRAY_COLOR;fillcolor = GRAY_COLOR;break;
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

	write_pixel(verticalBoundPairStart-4,yPosVerticalTopBound+4, bordercolor);
	write_pixel(verticalBoundPairStart-3,yPosVerticalTopBound+4, bordercolor);
	write_pixel(verticalBoundPairEnd+3,yPosVerticalTopBound+4, bordercolor);
	write_pixel(verticalBoundPairEnd+4,yPosVerticalTopBound+4, bordercolor);


	for(i=5;i<13;i++){
		write_pixel(verticalBoundPairStart-4,yPosVerticalTopBound+i, bordercolor);
		write_pixel(verticalBoundPairEnd+4,yPosVerticalTopBound+i, bordercolor);
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

	write_pixel(verticalBoundPairStart-4,yPosVerticalDownBound-4, bordercolor);
	write_pixel(verticalBoundPairStart-3,yPosVerticalDownBound-4, bordercolor);
	write_pixel(verticalBoundPairEnd+3,yPosVerticalDownBound-4, bordercolor);
	write_pixel(verticalBoundPairEnd+4,yPosVerticalDownBound-4, bordercolor);

	for(i=verticalBoundPairStart+1; i<verticalBoundPairEnd;i++){
		write_pixel(i, yPosVerticalTopBound+1,fillcolor);
		write_pixel(i, yPosVerticalDownBound-1,fillcolor);
	}
	for(i=verticalBoundPairStart; i<verticalBoundPairEnd+1;i++){
		write_pixel(i, yPosVerticalTopBound+2,fillcolor);
		write_pixel(i, yPosVerticalDownBound-2,fillcolor);
	}
	for(i=verticalBoundPairStart-1; i<verticalBoundPairEnd+2;i++){
		write_pixel(i, yPosVerticalTopBound+3,fillcolor);
		write_pixel(i, yPosVerticalDownBound-3,fillcolor);
	}
	for(i=verticalBoundPairStart-2; i<verticalBoundPairEnd+3;i++){
		write_pixel(i, yPosVerticalTopBound+4,fillcolor);
		write_pixel(i, yPosVerticalDownBound-4,fillcolor);
	}
	for(i=5;i<12;i++){
		for(j=verticalBoundPairStart-3; j<verticalBoundPairEnd+4;j++){
			write_pixel(j, yPosVerticalTopBound+i,fillcolor);
		}
	}

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