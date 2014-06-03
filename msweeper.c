#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/* curses ライブラリを使う際に必要なヘッダ */
#include	<curses.h>

#include "my_curses.h"
#include "msweeper.h"

#define MAP_MAX_ROW 100
#define MAP_MAX_COL 100

#define MAP_WALL -1
#define MAP_NONE 0
#define MAP_BOM 1

#define DISP_NONE 0
#define DISP_EXIST 1

void init_map(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int*, int*, int*);
void disp_game(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int, int);

void disp_game_cui(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int row_size, int col_size);
void create_map(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int, int, int);

int main(void){
	int map_row_size, map_col_size, bom;
	MAP map_data[MAP_MAX_ROW][MAP_MAX_COL];
	
	init_map(map_data, &map_row_size, &map_col_size, &bom);

	init_curses();
	clear();

	disp_game( map_data, map_row_size, map_col_size);
	while(1){
	}

	end_curses();

	return 0;
}

void init_map(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int *row_size, int *col_size, int *bom){
	printf("サイズを入力してください (x,y) :");
	scanf("%d,%d", row_size, col_size);
	printf("Mの数を入力してください :");
	scanf("%d",bom);

	printf(" x:%d, y:%d, box:%d\n この内容でゲームを開始します\n", *row_size, *col_size, *bom);

	int x,y;
	for(y=0; y<*col_size +2; y++){
		for(x=0; x<*row_size +2; x++){
			if(x == 0 || y == 0 || x == *row_size+1 || y == *col_size+1){
				map_data[x][y].data = MAP_WALL;
			}
			map_data[x][y].disp_flg = DISP_NONE;
		}
	}
	create_map(map_data, *row_size, *col_size, *bom);
}

void create_map(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int row_size, int col_size, int bom_num){
	int bom[10000];

	int i, pos, tmp;
	for(i=1; i<=row_size*col_size; i++){
		if(i<=bom_num)
			bom[i] = MAP_BOM;
		else
			bom[i] = MAP_NONE;
	}

	for(i=1; i<=bom_num; i++){
		pos = rand()%(row_size*col_size);

		tmp = bom[i];
		bom[i] = bom[pos];
		bom[pos] = tmp;
	}

	int x,y;
	for(y=1; y<col_size +1; y++){
		for(x=1; x<row_size +1; x++){
			map_data[x][y].data = bom[((y-1)*row_size)+x];
		}
	}
}

void disp_game(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int row_size, int col_size){
	int x,y;
	for(y=0; y<col_size+2; y++){
		for(x=0; x<row_size+2; x++){
			move(y,x*2);
			switch(map_data[x][y].data){
				case MAP_WALL:
					printw("++");
					break;
				case MAP_NONE:
					printw("0 ");
					break;
				case MAP_BOM:
					printw("* ");
					break;
				default:
					printw("8 ");
					break;
			}
		}
	}
	refresh();
}
