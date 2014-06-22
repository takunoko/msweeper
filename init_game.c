#include <time.h>
#include <stdlib.h>
#include <curses.h>

#include "ms_game.h"
#include "init_game.h"

void init_app(){
	srand((unsigned)time(NULL));
	init_char_color();
}

void init_map(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int *row_size, int *col_size, int *bom, int *pos_x, int *pos_y){
	int inputchar = '0';
	int select;

	*row_size = *col_size = *bom = 10;

	disp_default_menu( *row_size, *col_size, *bom);
	while(inputchar != ' '){
		inputchar = input_key();
		change_val( inputchar, &select, row_size, col_size, bom);
		refresh();
	}

	*pos_x = *row_size/2, *pos_y = *col_size/2;

	create_map(map_data, *row_size, *col_size);
	create_bom(map_data, *row_size, *col_size, *bom);
	create_map_num(map_data, *row_size, *col_size);
}

void change_val(int inputchar, int *select, int *row_size, int *col_size, int *bom){
	// 行を選択
	int max_bom;
	max_bom = (*row_size) * (*col_size) - 1;

	switch(inputchar){	
		case KEY_RIGHT:
			*select = (*select+1) % SELECT_SIZE;	break;
		case KEY_LEFT:
			*select = (*select+SELECT_SIZE-1) % SELECT_SIZE;	break;
	}
	// 値の変更
	switch(*select){
		case 0:
			if(inputchar == KEY_UP)
				*row_size = ((*row_size) % (MAP_MAX_ROW-2))+1;
			else if(inputchar == KEY_DOWN)
				*row_size = (((*row_size)+(MAP_MAX_ROW-3-1)) % (MAP_MAX_ROW-2))+1;
			mvprintw(12,10,"%4d",*row_size);
			break;
		case 1:
			if(inputchar == KEY_UP)
				*col_size = ((*col_size) % (MAP_MAX_COL-2))+1;
			else if(inputchar == KEY_DOWN)
				*col_size = (((*col_size)+(MAP_MAX_COL-3-1)) % (MAP_MAX_COL-2))+1;
			mvprintw(12,20,"%4d",*col_size);
			break;
		case 2:
			if(inputchar == KEY_UP)
				*bom = ((*bom) % (max_bom))+1;
			else if(inputchar == KEY_DOWN)
				*bom = (((*bom)+(max_bom-2)) % max_bom)+1;
			mvprintw(12,30,"%4d",*bom);
			break;
	}
}

void disp_default_menu(int row_size, int col_size, int bom){
	clear();
	attron(COLOR_PAIR(DEFAULT_COLOR));
	
	mvprintw( 1, 3, "          ===========               ");
	mvprintw( 2, 3, "          = Msweeper=               ");
	mvprintw( 3, 3, "          ===========               ");
	mvprintw( 4, 3, "                                    ");
	mvprintw( 5, 3, "                                    ");
	mvprintw( 6, 3, "                    j11-407         ");
	mvprintw( 7, 3, "                      Ezawa Takuya  ");
	mvprintw( 8, 3, "                                    ");
	mvprintw( 9, 3, "			                               ");

	mvprintw(15,10,"Use arrow key and value change.");
	mvprintw(16,10,"Start game\t: Space key");	

	mvprintw(11,21,"col");
	mvprintw(12,20,"%4d",col_size);
	mvprintw(11,31,"bom");
	mvprintw(12,30,"%4d",bom);
	//カーソルの位置をrowに合わせる手間を省くために、最後にrowを表示
	mvprintw(11,11,"row");
	mvprintw(12,10,"%4d",row_size);
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

void create_map(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int row_size, int col_size){
	int x,y;
	for(y=0; y<col_size +2; y++){
		for(x=0; x<row_size +2; x++){
			if(x == 0 || y == 0 || x == row_size+1 || y == col_size+1){
				map_data[x][y].data = MAP_WALL;
			}
			map_data[x][y].disp_flg = MAP_CLOSE;
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
			if(map_data[pos_x-1][pos_y].data == MAP_BOM)
				risk++;
		}
		// 右
		if(pos_x < row_size+1){
			if(map_data[pos_x+1][pos_y].data == MAP_BOM)
				risk++;
		}
		// 上
		if(pos_y > 1){
			if(map_data[pos_x][pos_y-1].data == MAP_BOM)
				risk++;
			if(pos_x > 1 && map_data[pos_x-1][pos_y-1].data == MAP_BOM)
					risk++;
			if(pos_x < row_size+1 && map_data[pos_x+1][pos_y-1].data == MAP_BOM)
				risk++;
		}
		// 下
		if(pos_y < col_size+1){
			if(map_data[pos_x][pos_y+1].data == MAP_BOM)
				risk++;
			if(pos_x > 1 && map_data[pos_x-1][pos_y+1].data == MAP_BOM)
				risk++;
			if(pos_x < row_size+1 && map_data[pos_x+1][pos_y+1].data == MAP_BOM)
				risk++;
		}	
	}else{
		risk = MAP_BOM;
	}

	if(risk == 0)
		risk = MAP_NONE;

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
	init_pair( MAP_WALL, COLOR_BLACK, COLOR_WHITE);
	init_pair( MAP_BOM, COLOR_RED, COLOR_RED);
	init_pair( MAP_CLOSE, COLOR_WHITE, COLOR_WHITE);

	init_pair( GAME_OVER, COLOR_WHITE, COLOR_RED);
	init_pair( GAME_CLEAR, COLOR_WHITE, COLOR_BLUE);
}
