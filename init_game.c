#include <time.h>
#include <stdlib.h>
#include <curses.h>

#include "msweeper.h"
#include "init_game.h"

void init_app(){
	srand((unsigned)time(NULL));
	init_char_color();
}

void init_map(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int *row_size, int *col_size, int *bom, int *pos_x, int *pos_y){
	// printf("サイズを入力してください (x,y) :");
	// scanf("%d,%d", row_size, col_size);
	// printf("Mの数を入力してください :");
	// scanf("%d",bom);

	// printf(" x:%d, y:%d, box:%d\n この内容でゲームを開始します\n", *row_size, *col_size, *bom);

	/* デバッグ用 */
	*row_size = 8;
	*col_size = 6;
	*bom = 10;

	*pos_x = *row_size/2;
	*pos_y = *col_size/2;

	int x,y;
	for(y=0; y<*col_size +2; y++){
		for(x=0; x<*row_size +2; x++){
			if(x == 0 || y == 0 || x == *row_size+1 || y == *col_size+1){
				map_data[x][y].data = MAP_WALL;
			}
			map_data[x][y].disp_flg = MAP_CLOSE;
		}
	}
	create_bom(map_data, *row_size, *col_size, *bom);
	create_map_num(map_data, *row_size, *col_size);
}

void create_bom(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int row_size, int col_size, int bom_num){
	int bom[10000];

	int i, pos, tmp;
	//ボムを1次元配列に代入しシャッフルする
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

void create_map_num(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int row_size, int col_size){
	int x,y;
	for(y=1; y<col_size +1; y++){
		for(x=1; x<row_size +1; x++){
			map_data[x][y].data = get_risk( map_data, x, y, row_size, col_size);
		}
	}
}

int get_risk(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int pos_x, int pos_y, int row_size, int col_size){
	int risk=0;

	if(map_data[pos_x][pos_y].data != MAP_BOM){
		// 左
		if(pos_x > 1){
			if(map_data[pos_x-1][pos_y].data == MAP_BOM){
				risk++;
			}
		}
		// 右
		if(pos_x < row_size+1){
			if(map_data[pos_x+1][pos_y].data == MAP_BOM){
				risk++;
			}
		}
		// 上
		if(pos_y > 1){
			if(map_data[pos_x][pos_y-1].data == MAP_BOM){
				risk++;
			}
			if(pos_x > 1){
				if(map_data[pos_x-1][pos_y-1].data == MAP_BOM)
					risk++;
			}
			if(pos_x < row_size+1){
				if(map_data[pos_x+1][pos_y-1].data == MAP_BOM)
					risk++;
			}
		}
		// 下
		if(pos_y < col_size+1){
			if(map_data[pos_x][pos_y+1].data == MAP_BOM)
				risk++;
			if(pos_x > 1){
				if(map_data[pos_x-1][pos_y+1].data == MAP_BOM)
					risk++;
			}
			if(pos_x < row_size+1){
				if(map_data[pos_x+1][pos_y+1].data == MAP_BOM)
					risk++;
			}
		}	
	}else{
		risk = MAP_BOM;
	}

	return risk;
}

void init_char_color(){
	init_pair( 1, COLOR_CYAN, COLOR_BLACK);
	init_pair( 2, COLOR_BLUE, COLOR_BLACK);
	init_pair( 3, COLOR_GREEN, COLOR_BLACK);
	init_pair( 4, COLOR_YELLOW, COLOR_BLACK);
	init_pair( 5, COLOR_RED, COLOR_BLACK);
	init_pair( 6, COLOR_MAGENTA,COLOR_BLACK);
	init_pair( 7, COLOR_MAGENTA, COLOR_BLACK);
	init_pair( 8, COLOR_MAGENTA, COLOR_BLACK);

	init_pair( MAP_NONE, COLOR_WHITE, COLOR_BLACK);
	init_pair( MAP_WALL, COLOR_WHITE, COLOR_WHITE);
	init_pair( MAP_BOM, COLOR_RED, COLOR_RED);
	init_pair( MAP_CLOSE, COLOR_WHITE, COLOR_WHITE);
}
