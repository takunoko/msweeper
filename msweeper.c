#include <stdio.h>
#include <stdlib.h>
/* curses ライブラリを使う際に必要なヘッダ */
#include	<curses.h>

typedef struct{
	int data;
	int disp_flg;
}MAP;

void init_map(MAP **, int*, int*, int*);

int main(void){
	int map_size_x, map_size_y, bom;
	//MAP **map_data;
	MAP map_data[100][100];
	
	int x=10, y=20;
	char c, oldc;

	init_curses();

	clear();
	move(10,10);
	addstr("aaaa");
	refresh();

	end_curses();

	return 0;
}

void init_map(MAP **map_data, int *x, int *y, int *bom){
	printf("サイズを入力してください (x,y) :");
	scanf("%d,%d", x, y);
	printf("Mの数を入力してください :");
	scanf("%d",bom);

	printf(" x:%d, y:%d, bom:%d\n この内容でゲームを開始します\n", *x, *y, *bom);
}

void del_map(MAP **map_data, int x, int y){
}

void disp_game( int size_x, int size_y, MAP map_data){
	move(10,10);
	addstr("*** testcurses ***");
}
