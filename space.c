#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>

int WIDTH , HEIGHT , size;
int PLAYER_X;
int PLAYER_Y;
int N_OBS=20 , N_BULS=100;
int SCORE = 0;
int running=1;

typedef struct{
	// x,y is the top left coordinate of obsticle
	// width , height is used to calculate the bounding box for collision detection
	int x;
	float y;
	int width ;
	int height;
	int active;
} Obsticle;
typedef struct{
	int x;
	int y;
	int active;
} Bullet;
void event_listener (Bullet bullet[]);
int did_bullet_Obsticle_collide(Bullet bull , Obsticle obs);
void clear(char buff[]);
void update(char buff[],Bullet bullets[],Obsticle obs[]);
void draw(char buff[]);
void draw_point(char buff[] , int x ,int y , char sym);
void get_console_size();
int getRand(int min,int max);
int main(){
	get_console_size();
	char BUFFER[size];
	clear(BUFFER);
	PLAYER_X = WIDTH/2;
	PLAYER_Y = HEIGHT -2;
	Bullet bullets[N_BULS]; // height = y , value stored = x

	Obsticle obs[N_OBS];

	for(int i=0;i<20;i++){
		obs[i].x=0;
		obs[i].y=0;
		obs[i].width=3;
		obs[i].height=3;
		obs[i].active=-1;
	}

	for(int i=0;i<N_BULS;i++){
		bullets[i].x=-1;
		bullets[i].y=-1;
		bullets[i].active=-1;
	}
	// GAMELOOP
	while(running){ 
		event_listener(bullets);
		clear(BUFFER);
		update(BUFFER,bullets,obs);
		draw(BUFFER);
		Sleep(100);
	}
	system("cls");
	printf("\n\n\n\t\t\t\tGAME OVER!!");

	return 0;
}


void event_listener (Bullet bullets[]){
	if(GetAsyncKeyState(VK_UP)){
		for(int i=0;i<N_BULS;i++){
			if(bullets[i].active==1 && (bullets[i].x==PLAYER_X && bullets[i].y==PLAYER_Y-3)){
				break;
			}
		}
		for(int i=0;i<N_BULS;i++){
			if(bullets[i].active==-1){
				bullets[i].x=PLAYER_X;
				bullets[i].y=PLAYER_Y-1;
				bullets[i].active=1;
				break;
			}
		}
	}
	if(GetAsyncKeyState(VK_DOWN)){
		
	}
	if(GetAsyncKeyState(VK_LEFT)){
		if(PLAYER_X>1){
			PLAYER_X-=2;
		}
	}
	if(GetAsyncKeyState(VK_RIGHT)){
		if(PLAYER_X<WIDTH-2){
			PLAYER_X+=2;
		}
	}
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

int did_bullet_Obsticle_collide(Bullet bull , Obsticle obs){
	if((bull.active==-1)||(obs.active==-1))
		return 0;
	return bull.x>=obs.x && bull.x<=(obs.x+obs.width-1) && bull.y>=obs.y && bull.y<=(obs.y+obs.height-1) ;
}


void update(char buff[],Bullet bullets[],Obsticle obs[N_OBS]){
	int i,j,k;
	// draw player on buffer
	draw_point(buff,PLAYER_X-1,PLAYER_Y,'<');
	draw_point(buff,PLAYER_X,PLAYER_Y,'=');
	draw_point(buff,PLAYER_X+1,PLAYER_Y,'>');
	draw_point(buff,PLAYER_X,PLAYER_Y-1,'^');
	// move objects
	for(i=0;i<N_OBS;i++){
		if(obs[i].active!=1)
			continue;
		obs[i].y+=0.25;
		if(obs[i].y+2>HEIGHT){
			obs[i].active=-1;
			running=0;
		}
		// draw objects
		for(j=0;j<obs[i].width;j++){
			for(k=0;k<obs[i].height;k++){
				draw_point(buff,obs[i].x+j,(int)(obs[i].y)+k,'#');
			}
		}
	}
	// update bullet position
	for(i=0;i<N_BULS;i++){
		if(bullets[i].active==-1){
			continue;
		}else if(bullets[i].y<0){
			bullets[i].active=-1;
		}
		else{
			bullets[i].y--;
		}
	}
	// collide and destroy
	for(i=0;i<N_OBS;i++){
		for(j=0;j<N_BULS;j++){
			if(bullets[j].active==1){
				if(did_bullet_Obsticle_collide(bullets[j],obs[i])){
					bullets[j].active=-1;
					obs[i].active=-1;
					SCORE++;
				}
			}
		}
	}
	// draw bullets on buffer
	for(i=0;i<N_BULS;i++){
		if(bullets[i].active==-1)
			continue;
		draw_point(buff,bullets[i].x,bullets[i].y,'"');
	}
	// Printing SCORE
	draw_point(buff,0,0,'S');
	draw_point(buff,1,0,'C');
	draw_point(buff,2,0,'O');
	draw_point(buff,3,0,'R');
	draw_point(buff,4,0,'E');
	draw_point(buff,5,0,':');
	int temp=SCORE;
	for(i=9;temp>0;i--){
		j=temp%10;
		temp/=10;
		draw_point(buff,i,0,(char)48+j);
	}

	// create obstecls
	if(getRand(0,100000)%2==0){
		int rand = getRand(-3*WIDTH,WIDTH);
		if(rand>3&&rand<WIDTH-3){
			for(i=0;i<N_OBS;i++){
				if(obs[i].active==-1){
					obs[i].x=rand;
					obs[i].y=-2;
					obs[i].active=1;
					break;
				}
			}
		}
	}


}


void draw(char buff[]){
	HANDLE screen = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD written;
	COORD coord ={0,0};

	WriteConsoleOutputCharacter(screen,buff,size,coord,&written);
	
}

void draw_point(char buff[] , int x ,int y , char sym){
	if(x<0 || x>WIDTH || y<0 || y>HEIGHT||(x + (y*WIDTH))>size){
		return;
	}
	buff [x + (y*WIDTH)] = sym ;
}

void get_console_size(){
	HANDLE screen = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD count;
	DWORD cellCount;
	COORD coord ={0,0};

	if(!GetConsoleScreenBufferInfo(screen,&csbi)){
		WIDTH = 80;
		HEIGHT = 25;
	}
	WIDTH = csbi.srWindow.Right -  csbi.srWindow.Left +1;
	HEIGHT = csbi.srWindow.Bottom -  csbi.srWindow.Top +1;
	size = WIDTH*HEIGHT;

}
int getRand(int min,int max){
	return (rand()% (max-min+1))+min;
}