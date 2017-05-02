/*					*/
/*	 john paul ibay 07-67611	*/
/*      angelo de la vina 07-67724	*/
/*					*/
/*	LIGHTSOUT! Game for ICS-OS	*/


#include "../../sdk/dexsdk.h"
#include "../../sdk/time.h"

void set_coordinates(int x, int y); //initialize bulb coordinates
void setup_level(); //self explanatory
void flip_fxn(int x, int y); 
void gray(int x, int y); //prints a gray bulb socket
void light(int r, int c, int x, int y); //prints a bulb light
void print_bulb(int r, int c, int x, int y); //print a bulb
void print_board(int x, int y); //set up initial board
void edit_board(); // update selected and previously selected bulb colors
void erase(); //basically covers an area with a black rectangle

/* constants */
#define maxrow 5
#define maxcol 5
#define max_level 6
#define pause_length 25

#define on 1
#define son 2
#define off 3
#define soff 4
#define flip 'l'
#define up_key 'w'
#define down_key 's'
#define left_key 'a'
#define right_key 'd'
#define quit 'x'
#define reset 'r'
#define yes 'y'
#define no 'n'
#define start '1'
#define quit_game '2'

#define YELLOW 54
#define PALE_YELLOW 62
#define ROYAL_BLUE 1
#define DARK_BLUE 8
#define GRAY 56
#define WHITE 63

#define X_coord 85
#define Y_coord 35

/* global variables */
int row, col, oldrow=0, oldcol=0;
int flips=0, remaining_bulb;
int level, count=0, hour=0, min=0;
char board[maxrow][maxcol];
int bulbs_x[maxrow][maxcol];
int bulbs_y[maxrow][maxcol];

int main()
{
  
	char keypress = start;
	int i,j;
	char str[15];
	int champion = 0;

	set_graphics(VGA_320X200X256);
  	set_coordinates(X_coord, Y_coord); //initialize bulb coordinates

	do{
	erase(1,1,400,220);

  	header(8,5); //print menu

	level=1; //initialize level
	champion = 0; //falsify championhood
	
	keypress=(char)getch();
	erase(1,1,400,220); //erase menu

	if(keypress==start){
	do{

		if(keypress == start){
			setup_level(); 
			row=0;
			col=0;
			flips=0;

			//update level display
			erase(25,125,40,30);
			write_text("0",25,125,WHITE,0);

		}		
		else
			print_board(X_coord, Y_coord);

		do{

			if (keypress=(char)getch()){
			
				// update selected bulb
				if(keypress==right_key){
					oldrow = row;
					oldcol = col;
					col = (col + 1) % maxcol;
				}
				
				else if(keypress==left_key){
					oldrow = row;
					oldcol = col;
					if(col <= 0)
						col = maxcol - 1;
					else
						col--;
				}
				
				else if(keypress==up_key){
					oldrow = row;
					oldcol = col;keypress=(char)getch();
					if(row <= 0)
						row = maxrow - 1;
					else
						row--;
				}
				
				else if(keypress==down_key){
					oldrow = row;
					oldcol = col;
					row = (row + 1) % maxrow;
				}
				
				else if(keypress == flip){				
					flips++;
					erase(25,125,30,30);
					sprintf(str,"%d",flips);
				   	write_text(str,25,125,WHITE,0);
					
					// flip bulbs
				
					// selected bulb
					if(board[row][col] == son){
						board[row][col] = soff;
						remaining_bulb--;
						flip_fxn(row, col);
					}
					else{
						board[row][col] = son;
						remaining_bulb++;
						flip_fxn(row, col);
					}
					
					// top
					if(row -1 >= 0){
						if(board[row-1][col] == on){
							board[row-1][col] = off;
							remaining_bulb--;
							flip_fxn(row-1, col);
						}
						else{
							board[row-1][col] = on;
							remaining_bulb++;
							flip_fxn(row-1, col);
						}
					}
					
					// down
					if(row + 1 < maxrow){
						if(board[row+1][col] == on){
							board[row+1][col] = off;
							remaining_bulb--;
							flip_fxn(row+1, col);
						}
						else{
							board[row+1][col] = on;
							remaining_bulb++;
							flip_fxn(row+1, col);
						}
					}
					
					// left
					if(col -1 >= 0){
						if(board[row][col-1] == on){
							board[row][col-1] = off;
							remaining_bulb--;
							flip_fxn(row, col-1);
						}
						else{
							board[row][col-1] = on;
							remaining_bulb++;
							flip_fxn(row, col-1);
						}
					}
					
					// right
					if(col + 1 < maxcol){
						if(board[row][col+1] == on){
							board[row][col+1] = off;
							remaining_bulb--;
							flip_fxn(row, col+1);
						}
						else{
							board[row][col+1] = on;
							remaining_bulb++;
							flip_fxn(row, col+1);
						}
					}
					
				}
				
				if(remaining_bulb <= 0){

					level++; //update level
					if(level==max_level){ //finished all puzzles

						write_text("Congratulations!",89,160,YELLOW,0);
						write_text("You finished all levels.",65,170,YELLOW,0);
						write_text("(Press any key)",101, 180,YELLOW,0);
						keypress=(char)getch();
						erase(1,1,350,280); //clear screen
						champion=1; //player is now champion

					}else{ //finished a puzzle
						write_text("Congratulations!",89,160,WHITE,0);
						write_text("Press any key",101, 170,WHITE,0);
						keypress=(char)getch();
						erase(80,160,150,40); //erase congratulations
						keypress = start;
					}
					break;
				}
				
				// update the display of the selected and previously selected
				if(keypress==right_key || keypress==left_key || keypress==up_key || keypress==down_key){
					board[row][col] = board[row][col] == on? son: soff;
					board[oldrow][oldcol] = board[oldrow][oldcol] == son? on: off;					
					edit_board();
				}
			}			
	      		
		} while (keypress != quit && keypress != reset && champion!=1);
			//continue while player is not quitting, restarting or champion

		if(champion==1){keypress = yes;}
		else if(keypress == quit){
			//prompt confirmation then erase message
			write_text("Do you want to exit? y/n ",60,160,WHITE,0); 
			keypress=(char)getch();
			erase(60,160,220,40);			
		}
		else if(keypress == reset){
			//prompt confirmation then erase message
			write_text("Do you want to restart? y/n ",50,160,WHITE,0);
			keypress=(char)getch();
			if(keypress == yes) keypress = start;
			erase(50,160,260,40);			
		}
		
		

	} while (keypress != yes);
	}
	} while (keypress != quit_game);

	set_graphics(VGA_TEXT80X25X16);
	clrscr();
}

void flip_fxn(int x, int y){

	light(x, y, bulbs_x[x][y], bulbs_y[x][y]); 
}


void set_coordinates(int x, int y){ //initialize bulb coordinates
	int i, j, a, b;
	
	a = x;
	b = y;
	
	for(i=0; i<maxrow; i++, b+=24){
		for(j=0; j<maxcol; j++, a+=31){
			bulbs_x[i][j] = a;
			bulbs_y[i][j] = b;
		}
		a=x;
	}
}


void setup_level(){ //self explanatory
 
	int i, j;
   
	for(i=0; i<maxrow; i++)
		for(j=0; j<maxcol; j++)
			board[i][j]= off;

	switch(level){
	
		case 1:			
			board[0][0] = on;
			board[0][1] = on;
			board[0][3] = on;
			board[0][4] = on;

			board[1][0] = on;
			board[1][2] = on;
			board[1][4] = on;

			board[2][1] = on;
			board[2][2] = on;
			board[2][3] = on;

			board[3][0] = on;
			board[3][2] = on;
			board[3][4] = on;

			board[4][0] = on;
			board[4][1] = on;
			board[4][3] = on;
			board[4][4] = on;
			break;

		case 2:
			
			board[0][1] = on;
			board[0][3] = on;
			
			board[1][0] = on;
			board[1][1] = on;
			board[1][3] = on;
			board[1][4] = on;
			
			board[2][1] = on;
			board[2][3] = on;
			
			board[3][0] = on;
			board[3][2] = on;
			board[3][4] = on;
			
			board[4][0] = on;
			board[4][2] = on;
			board[4][4] = on;
			break;
	
		case 3:
		
			board[0][0] = on;
			board[0][4] = on;
			
			board[1][0] = on;
			board[1][1] = on;
			board[1][3] = on;
			board[1][4] = on;
			
			board[2][2] = on;
			
			board[3][0] = on;
			board[3][2] = on;
			
			board[4][0] = on;
			board[4][2] = on;
			board[4][3] = on;
			break;

		case 4:
		
			board[0][0] = on;
			board[0][1] = on;
			board[0][3] = on;
			board[0][4] = on;
						
			board[2][0] = on;
			board[2][1] = on;
			board[2][3] = on;
			board[2][4] = on;
			
			board[3][4] = on;
			
			board[4][0] = on;
			board[4][1] = on;
			break;
			
		case 5:
			
			board[0][0] = on;
			board[0][1] = on;
			board[0][2] = on;
			board[0][3] = on;
			board[0][4] = on;
			
			board[1][0] = on;
			board[1][1] = on;
			board[1][2] = on;
			board[1][3] = on;
			board[1][4] = on;
			
			board[2][0] = on;
			board[2][1] = on;
			board[2][2] = on;
			board[2][3] = on;
			board[2][4] = on;
			
			board[3][0] = on;
			board[3][1] = on;
			board[3][2] = on;
			board[3][3] = on;
			board[3][4] = on;
			
			board[4][0] = on;
			board[4][1] = on;
			board[4][2] = on;
			board[4][3] = on;
			board[4][4] = on;
			break;

		case 6: board[0][3] = on;
			board[0][4] = on;
			board[1][3] = on;
			board[1][4] = on;
			board[3][0] = on;
			board[3][1] = on;
			board[4][0] = on;
			board[4][1] = on;
			break;

		case 7: board[1][1] = on;
			board[1][2] = on;
			board[1][3] = on;
			board[2][0] = on;
			board[2][1] = on;
			board[2][2] = on;
			board[2][3] = on;
			board[2][4] = on;
			board[3][1] = on;
			board[3][2] = on;
			board[3][3] = on;
			break;

		case 8: board[1][1] = on;
			board[1][2] = on;
			board[1][3] = on;
			board[2][1] = on;
			board[2][2] = on;
			board[2][3] = on;
			board[3][1] = on;
			board[3][2] = on;
			board[3][3] = on;
			break;

		case 9: 

			board[0][0] = on;
			board[0][1] = on;
			board[0][3] = on;
			board[0][4] = on;
			board[1][0] = on;
			board[1][1] = on;
			board[1][3] = on;
			board[1][4] = on;
			board[3][0] = on;
			board[3][1] = on;
			board[3][3] = on;
			board[3][4] = on;
			board[4][0] = on;
			board[4][1] = on;
			board[4][3] = on;
			board[4][4] = on;
			break;

		case 10:
			board[0][0] = on;
			board[0][1] = on;
			board[0][2] = on;
			board[0][3] = on;
			board[0][4] = on;
			board[1][1] = on;
			board[1][2] = on;
			board[1][3] = on;
			board[2][2] = on;
			board[4][0] = on;
			board[4][1] = on;
			board[4][2] = on;
			board[4][3] = on;
			board[4][4] = on;
			board[3][1] = on;
			board[3][2] = on;
			board[3][3] = on;
			break;
	}	
	
	// counts the number of bulbs that are turned on
	remaining_bulb = 0;
	for(i=0; i<maxrow; i++)
		for(j=0; j<maxcol; j++)
			if(board[i][j] == on)
				remaining_bulb++;
	
	// starting point
	board[0][0] = board[0][0] == on? son : soff;

	erase(190,5,30,20); //erase menu
	print_board(85, 35);
}


void gray(int x, int y){ //prints a gray bulb socket 
int i;
	for (i=8;i<18;i++)write_pixel(i+x,0+y,56);
	for (i=6;i<20;i++)write_pixel(i+x,1+y,56);
	for (i=4;i<21;i++)write_pixel(i+x,2+y,56);
	for (i=3;i<22;i++)write_pixel(i+x,3+y,56);
	for (i=2;i<23;i++)write_pixel(i+x,4+y,56);
	for (i=1;i<24;i++)write_pixel(i+x,5+y,56);
	for (i=1;i<25;i++)write_pixel(i+x,6+y,56);
	for (i=0;i<25;i++)write_pixel(i+x,7+y,56);
	for (i=0;i<25;i++)write_pixel(i+x,8+y,56);
	for (i=0;i<25;i++)write_pixel(i+x,9+y,56);
	for (i=0;i<25;i++)write_pixel(i+x,10+y,56);
	for (i=0;i<25;i++)write_pixel(i+x,11+y,56);
	for (i=1;i<24;i++)write_pixel(i+x,12+y,56);
	for (i=1;i<24;i++)write_pixel(i+x,13+y,56);
	for (i=2;i<23;i++)write_pixel(i+x,14+y,56);
	for (i=3;i<22;i++)write_pixel(i+x,15+y,56);
	for (i=5;i<20;i++)write_pixel(i+x,16+y,56);
	for (i=7;i<18;i++)write_pixel(i+x,17+y,56);
	for (i=9;i<15;i++)write_pixel(i+x,18+y,56);

}

void light(int r, int c, int x, int y){ //prints a bulb light 
	int i, color;
	
	switch(board[r][c]){ //sets the bulb's color
		case on: color = YELLOW; break;
		case son: color = WHITE; break;
		case off: color = DARK_BLUE; break;
		case soff: color = BLUE; break;
	}
	
	for (i=9;i<17;i++)write_pixel(i+x,2+y,color);
	for (i=7;i<19;i++)write_pixel(i+x,3+y,color);
	for (i=5;i<20;i++)write_pixel(i+x,4+y,color);
	for (i=4;i<21;i++)write_pixel(i+x,5+y,color);
	for (i=4;i<21;i++)write_pixel(i+x,6+y,color);
	for (i=3;i<22;i++)write_pixel(i+x,7+y,color);
	for (i=3;i<22;i++)write_pixel(i+x,8+y,color);
	for (i=4;i<21;i++)write_pixel(i+x,9+y,color);
	for (i=4;i<21;i++)write_pixel(i+x,10+y,color);
	for (i=5;i<20;i++)write_pixel(i+x,11+y,color);
	for (i=7;i<18;i++)write_pixel(i+x,12+y,color);
	for (i=9;i<17;i++)write_pixel(i+x,13+y,color);
}

void print_bulb(int r, int c, int x, int y){ //print a bulb 

	gray(x, y);
	light(r, c, x, y);
	
}


void print_board(int x, int y){ //set up initial board 

	int i, j, a, b;
 	a=x;
 	b=y;
 	char str[15];

	//display level
   	write_text("Level",135,5,WHITE,0); 
	sprintf(str,"%d",level);
   	write_text(str,190,5,WHITE,0);

	//print the 25 bulbs
	for(i=0; i<maxrow; i++, b+=24){
		for(j=0; j<maxcol; j++, a+=31)
			print_bulb(i, j, a, b);
		a=x;
	}
	
	//display legend
	write_text("Up-W",5,35,WHITE,0);
	write_text("Dn-S",5,45,WHITE,0);
	write_text("Lf-A",5,55,WHITE,0);
	write_text("Rt-D",5,65,WHITE,0);
	
	write_text("Flip-L",5,75,WHITE,0);
	write_text("Exit-X",5,85,WHITE,0);
	write_text("Reset-R",5,95,WHITE,0);

	//show number of flips
	write_text("Flips:",5,115,WHITE,0);
	
}

// update selected and previously selected bulb colors 
void edit_board(){

	light(oldrow, oldcol, bulbs_x[oldrow][oldcol], bulbs_y[oldrow][oldcol]);
	light(row, col, bulbs_x[row][col], bulbs_y[row][col]);
}

void erase(int x, int y, int w, int h){ //basically covers an area with a black rectangle 
   int i,j;
   for (i=y;i<=(y+h);i++)
      for (j=x;j<=(x+w);j++)
         write_pixel(j,i,100);
}

void light2(int x, int y, int color){ //prints bulb light without checking,
	int i;			      //explicit color defining	
	
	for (i=9;i<17;i++)write_pixel(i+x,2+y,color);
	for (i=7;i<19;i++)write_pixel(i+x,3+y,color);
	for (i=5;i<20;i++)write_pixel(i+x,4+y,color);
	for (i=4;i<21;i++)write_pixel(i+x,5+y,color);
	for (i=4;i<21;i++)write_pixel(i+x,6+y,color);
	for (i=3;i<22;i++)write_pixel(i+x,7+y,color);
	for (i=3;i<22;i++)write_pixel(i+x,8+y,color);
	for (i=4;i<21;i++)write_pixel(i+x,9+y,color);
	for (i=4;i<21;i++)write_pixel(i+x,10+y,color);
	for (i=5;i<20;i++)write_pixel(i+x,11+y,color);
	for (i=7;i<18;i++)write_pixel(i+x,12+y,color);
	for (i=9;i<17;i++)write_pixel(i+x,13+y,color);
}

bulb_row2(int x, int y){ //displays header top and bottom borders

	gray(0+x,0+y);
	light2(0+x,0+y,BLUE);

	gray(31+x,0+y);
	light2(31+x,0+y,YELLOW);

	gray(62+x,0+y);
	light2(62+x,0+y,BLUE);

	gray(93+x,0+y);
	light2(93+x,0+y,YELLOW);

	gray(124+x,0+y);
	light2(124+x,0+y,BLUE);

	gray(155+x,0+y);
	light2(155+x,0+y,YELLOW);

	gray(186+x,0+y);
	light2(186+x,0+y,BLUE);

	gray(217+x,0+y);
	light2(217+x,0+y,YELLOW);

	gray(248+x,0+y);
	light2(248+x,0+y,BLUE);

	gray(279+x,0+y);
	light2(279+x,0+y,YELLOW);


}

//displays header left and right borders
bulb_pair1(int x, int y){ 

	gray(0+x,0+y);
	light2(0+x,0+y,YELLOW);

	gray(279+x,0+y);
	light2(279+x,0+y,BLUE);
}

bulb_pair2(int x, int y){

	gray(0+x,0+y);
	light2(0+x,0+y,BLUE);

	gray(279+x,0+y);
	light2(279+x,0+y,YELLOW);
}

//displays header
header(int x, int y){

	bulb_row2(x,0+y);
	bulb_pair1(x,24+y);
	bulb_pair2(x,48+y);
	bulb_pair1(x,72+y);
	bulb_row2(x,96+y);
	
	write_text("LIGHTSOUT!",120,40,WHITE,1); //title

	//menu options
	write_text("1 - Start",40,160,WHITE,0); 
	write_text("2 - Quit",200,160,WHITE,0);
}


