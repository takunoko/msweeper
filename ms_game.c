#include <curses.h>

#include "msweeper.h"
#include "ms_game.h"
#include "init_game.h"

// 1回のゲームは、この中でループ
int game_loop(){
	int game_status;
	int input_char = ' ';

	MAP map_data[(MAP_MAX_ROW)][(MAP_MAX_COL)];
	int map_row_size, map_col_size, bom;
	int pos_x, pos_y;

	init_map(map_data, &map_row_size, &map_col_size, &bom, &pos_x, &pos_y);
	while(input_char != 'q'){
		disp_game( map_data, map_row_size, map_col_size, pos_x, pos_y);
		input_char = input_key();
		move_pos( map_data,  input_char, map_row_size, map_col_size, &pos_x, &pos_y);
	}

	return GAME_OVER;
}

// ゲーム画面の描画
void disp_game(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int row_size, int col_size, int pos_x, int pos_y){
	int x,y;
	for(y=0; y<col_size+2; y++){
		for(x=0; x<row_size+2; x++){
			switch(map_data[x][y].disp_flg){
			//switch(MAP_OPEN){
				case MAP_OPEN:
					attron(COLOR_PAIR(map_data[x][y].data));
					switch(map_data[x][y].data){
						case MAP_WALL:
							mvaddch(y,x*2,'+');
							break;
						case MAP_NONE:
							mvaddch(y,x*2,' ');
							break;
						case MAP_BOM:
							mvaddch(y,x*2,'*');
							break;
						default:
							mvaddch(y,x*2,map_data[x][y].data + '0');
							break;
					}
					break;

				case MAP_FLAG:
						attron(COLOR_PAIR(MAP_WALL));
						mvaddch(y,x*2,'F');
					break;
				
				default:
					switch(map_data[x][y].data){
						case MAP_WALL:
							attron(COLOR_PAIR(MAP_WALL));
							mvaddch(y,x*2,'+');
							break;
						default:
							attron(COLOR_PAIR(MAP_CLOSE));
							mvaddch(y,x*2,'#');
							break;
					}
					break;
			}
		}
		move( (pos_y+1), (pos_x+1)*2);

		refresh();
	}
}

// 入力されたキーをゲームように変換(ゲームで使用しないキーは入力不可)
int input_key(){
	int ch;
	int flg=0;
	do{
		ch = getch();
		switch(ch){
			case 'h':
				ch = KEY_LEFT;
				break;
			case 'j':
				ch = KEY_DOWN;
				break;
			case 'k':
				ch = KEY_UP;
				break;
			case 'l':
				ch = KEY_RIGHT;
				break;
			case KEY_LEFT:
			case KEY_RIGHT:
			case KEY_UP:
			case KEY_DOWN:
			case ' ':
			case 'f':
			case 'F':
			case 'q':
				break;
			default :
				flg = 1;
				break;
		}	
	}while(flg == 1);
	return ch;
}
// 移動イベント(キーボードイベント)
void move_pos(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int ch, int row_size, int col_size, int *pos_x, int *pos_y){
	switch(ch){
		case KEY_LEFT:
			*pos_x = (((*pos_x)+row_size-1)%row_size);
			break;
		case KEY_RIGHT:
			*pos_x = (((*pos_x)+1)%row_size);
			break;
		case KEY_UP:
			*pos_y = (((*pos_y)+col_size-1)%col_size);
			break;
		case KEY_DOWN:
			*pos_y = (((*pos_y)+1)%col_size);
			break;
		case ' ':
			map_open( map_data, (*pos_x)+1, (*pos_y)+1, row_size, col_size);
			break;
		case 'f':
		case 'F':
			switch(map_data[*pos_x+1][*pos_y+1].disp_flg){
				case MAP_OPEN:
					break;
				case MAP_FLAG:
					map_data[*pos_x+1][*pos_y+1].disp_flg = MAP_CLOSE;
					break;
				case MAP_CLOSE:
					map_data[*pos_x+1][*pos_y+1].disp_flg = MAP_FLAG;
					break;
			}
	}
}
// マップの開くが選択された時
void map_open(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int x, int y, int row_size, int col_size){
	if( map_data[x][y].data == MAP_BOM){
	}else{
		if(map_data[x][y].data == MAP_NONE){
			open_0( map_data, x, y);
			mvaddch(20,20,'i');
		}else{
			map_data[x][y].disp_flg = MAP_OPEN;
			mvaddch(20,20,'k');
		}
	}
}
// 0が続く限りマップを開き続ける
void open_0(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int x, int y){
	map_data[x][y].disp_flg = MAP_OPEN;
	// 上
	if(map_data[x][y-1].data != MAP_WALL){
		if(map_data[x][y-1].data == MAP_NONE && map_data[x][y-1].disp_flg != MAP_OPEN)
			open_0(map_data, x, y-1);
		else
			map_data[x][y-1].disp_flg = MAP_OPEN;
	}
	//左
	if(map_data[x-1][y].data != MAP_WALL){
		if(map_data[x-1][y].data == MAP_NONE && map_data[x-1][y].disp_flg != MAP_OPEN)
			open_0(map_data, x-1, y);
		else
			map_data[x-1][y].disp_flg = MAP_OPEN;
	}
	//下
	if(map_data[x][y+1].data != MAP_WALL){
		if(map_data[x][y+1].data == MAP_NONE && map_data[x][y+1].disp_flg != MAP_OPEN)
			open_0(map_data, x, y+1);
		else
			map_data[x][y+1].disp_flg = MAP_OPEN;
	}
	//右
	if(map_data[x+1][y].data != MAP_WALL){
		if(map_data[x+1][y].data == MAP_NONE && map_data[x+1][y].disp_flg != MAP_OPEN)
			open_0(map_data, x+1, y);
		else
			map_data[x+1][y].disp_flg = MAP_OPEN;
	}

	if(map_data[x-1][y-1].data != MAP_WALL){
		if(map_data[x-1][y-1].data== MAP_NONE && map_data[x-1][y-1].disp_flg != MAP_OPEN)
			open_0(map_data, x-1, y-1);
		else
			map_data[x-1][y-1].disp_flg = MAP_OPEN;
	}
	if(map_data[x-1][y+1].data != MAP_WALL){
		if(map_data[x-1][y+1].data == MAP_NONE && map_data[x-1][y+1].disp_flg != MAP_OPEN)
			open_0(map_data, x-1, y+1);
		else
			map_data[x-1][y+1].disp_flg = MAP_OPEN;
	}
	if(map_data[x+1][y+1].data != MAP_WALL){
		if(map_data[x+1][y+1].data == MAP_NONE && map_data[x+1][y+1].disp_flg != MAP_OPEN)
			open_0(map_data, x+1, y+1);
		else
			map_data[x+1][y+1].disp_flg = MAP_OPEN;
	}
	if(map_data[x+1][y-1].data != MAP_WALL){
		if(map_data[x+1][y-1].data == MAP_NONE && map_data[x+1][y-1].disp_flg != MAP_OPEN)
			open_0(map_data, x+1, y-1);
		else
			map_data[x+1][y-1].disp_flg = MAP_OPEN;
	}
}
