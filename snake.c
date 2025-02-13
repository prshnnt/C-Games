#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>

void to(int x , int y);
void clear(char buff[]);
void get_console_size();
void arrow_key();
void draw(char buff[]);
void draw_point(char buff[] , int x, int y , char sym );
void move_tail(int tail[][2],int tail_length,int curX,int curY);
void winner_display();

int width , height;
int size;

int main(){
	
	get_console_size();
	size = width*height;
	char buff[size] ;
	
	for(int i =0 ; i<size;i++){
		buff[i]=' ';
	}

	int curX=width/2,curY = height/2;
	int o_curX = curX , o_curY = curY;
	int tail_length = 1 , actual_tail_length = 20;
	int tail[actual_tail_length][2];
	for(int i =0 ; i<tail_length ;i++){
		tail[i][0] = curX ;
		tail[i][1] = curY + i;
	}
	
	
	int fx=10, fy=10;
	
	system("echo off");
	system("cls");
	int running = 1;
	while(running){
		clear(buff);
		o_curX = curX;
		o_curY = curY;
		arrow_key(&curX,&curY);
		// login for colliding with walls and 
		//not overlapping head of snake with next body part 
		if(
			(
				curX==tail[1][0] && curY==tail[1][1]
			)
			||
			(
				(curX>(width-2)||(curX<2))||
				(curY>(height-2)||(curY<1))
			)
		){
			curX=o_curX;
			curY=o_curY;
		}
		// increasing tail length on eating food and also randomly 
		//placing food after each consumption of food 
		if(curX==fx && curY==fy){
			if(!(tail_length>=actual_tail_length)){
				tail_length+=1;
				// rand()%(max-min+1) + min  
				//====> this is to get random number between max and min
				fx= rand()%(width-4) + 2;
				fy= rand()%(height-4) + 2;
			}else{
				running=0;
				clear(buff);
				winner_display();
			}
		}
		// maoving tail accoring to snake game logic
		if(o_curX!=curX || o_curY !=curY){
			move_tail(tail,tail_length,curX,curY);
		}
		// drawing snake tail in buff
		for(int i = 0 ; i<tail_length ; i++){
			draw_point(buff,tail[i][0],tail[i][1] , '*');
		}
		// drawing food in buff
		draw_point(buff,fx,fy,'@');

		//  drawing wallls in buff
		for(int j =0 ; j<height ; j++){
			if(j==0 || j==(height-1)){
				for(int i=0; i<width;i++){
					draw_point(buff,i,j,'#');
				}
			}
			draw_point(buff,0,j,'#');
			draw_point(buff,width-1,j,'#');
		}
		// drawing screen buffer
		draw(buff);
		// waiting after each frame draw
		// so to get smooth playing experience
		Sleep(100);
	}
	return 0;
}


void to(int x , int y){
	HANDLE screen = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	coord.X =x ;
	coord.Y =y ;
	SetConsoleCursorPosition(screen,coord);
	
}


void clear(char buff[]){
	HANDLE screen = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD count;
	DWORD cellCount;
	COORD coord ={0,0};

	if(!GetConsoleScreenBufferInfo(screen,&csbi)){
		return ;
	}

	cellCount = csbi.dwSize.X + csbi.dwSize.Y;

	// filling console white spaces
	FillConsoleOutputCharacter(screen,(TCHAR)' ',cellCount,coord,&count);

	//Reset attrbs
	FillConsoleOutputAttribute(screen,csbi.wAttributes,cellCount , coord , &count);

	// move cursor to (0,0) 
	SetConsoleCursorPosition(screen,coord);
	for(int i=0;i<size;i++){
		buff[i]=' ';
	}
}



void get_console_size(){
	HANDLE screen = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD count;
	DWORD cellCount;
	COORD coord ={0,0};

	if(!GetConsoleScreenBufferInfo(screen,&csbi)){
		width = 80;
		height = 25;
	}
	width = csbi.srWindow.Right -  csbi.srWindow.Left +1;
	height = csbi.srWindow.Bottom -  csbi.srWindow.Top +1;

}

void arrow_key(int *x , int *y){
	if(GetAsyncKeyState(VK_UP)){
		*y-=1;
	}
	if(GetAsyncKeyState(VK_DOWN)){
		*y+=1;
	}
	if(GetAsyncKeyState(VK_LEFT)){
		*x-=1;
	}
	if(GetAsyncKeyState(VK_RIGHT)){
		*x+=1;
	}
}


void draw(char buff[]){
	HANDLE screen = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD written;
	COORD coord ={0,0};

	WriteConsoleOutputCharacter(screen,buff,width*height,coord,&written);
	
}

void draw_point(char buff[] , int x ,int y , char sym){
	if((x + (y*width))>size){
		return;
	}
	buff [x + (y*width)] = sym ;
}

void move_tail(int tail[][2],int tail_length,int curX,int curY){
	for(int i =tail_length-1 ; i>0 ; i--){
		tail[i][0] = tail[i-1][0];
		tail[i][1] = tail[i-1][1];
	}
	tail[0][0]=curX;
	tail[0][1]=curY;
}

void winner_display(){
	for(int i=0;i<height/2;i++){
		printf("\n");
	}
	printf("WINNER WINNER WINNER !!!!!!!!!!!!!\n\n");
}















