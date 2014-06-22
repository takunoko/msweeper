#include <curses.h>
#include <unistd.h> // sleepを使うためのヘッダファル

#include "msweeper.h"
#include "ms_game.h"
#include "init_game.h"

// ゲームのループを何度も回す
void game_main(){
	int input_char = ' ';

	MAP map_data[(MAP_MAX_ROW)][(MAP_MAX_COL)];
	int pos_x, pos_y;
	int map_row_size, map_col_size, bom;

	int play_flg = 1;

	while(play_flg == 1){
		init_map(map_data, &map_row_size, &map_col_size, &bom, &pos_x, &pos_y);
		game_loop(map_data, map_row_size, map_col_size, pos_x, pos_y, bom);
	
		clear();
		attron(COLOR_PAIR(MAP_NONE));
		mvprintw(9,10, "    Next game?   ");
		mvprintw(10,10,"[Yes: y] [No : n]");

		do{
			input_char = input_key();
		}while(input_char != 'y' && input_char != 'n');
		if(input_char == 'n')
			play_flg = 0;
	}
	clear();
	mvprintw(10,10,"Thank you for playing!!!");
	refresh();
	sleep(2);
}

// 1回のゲームは、この中でループ
int game_loop(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int map_row_size, int map_col_size, int pos_x, int pos_y, int bom){
	int game_status = GAME_PLAY;
	int input_char = ' ';

	while(game_status == GAME_PLAY){
		disp_game( map_data, map_row_size, map_col_size, pos_x, pos_y);
		input_char = input_key();
		move_pos( map_data,  input_char, map_row_size, map_col_size, &pos_x, &pos_y);
		game_status = status_check( map_data, map_row_size, map_col_size, bom);

		switch (game_status){
			case GAME_OVER:
				game_over( map_data, map_row_size, map_col_size);
				break;
			case GAME_CLEAR:
				game_clear( map_data, map_row_size, map_col_size);
				break;
			default :
				break;
		}
	}

	//とりあえずgame_statusを返しておく
	return game_status;
}

// ゲーム画面の描画
void disp_game(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int row_size, int col_size, int pos_x, int pos_y){
	int x,y;
	
	clear();
	for(y=0; y<col_size+2; y++){
		for(x=0; x<row_size+2; x++){
			disp_dot(map_data, x, y);
		}
	}		

	attron(COLOR_PAIR(MAP_NONE));
	mvprintw( col_size + 3, 0, "move 'arrow key' or 'h,j,k,l'");
	mvprintw( col_size + 5, 0, "Map open : Space key");
	mvprintw( col_size + 7, 0, "Flag : F key");

	move( (pos_y+1), (pos_x+1)*2);
	refresh();
}

void disp_dot(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int x, int y){
	switch(map_data[x][y].disp_flg){
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

// 入力されたキーをゲームように変換(ゲームで使用しないキーは入力不可)
int input_key(){
	int ch;
	int flg=0;
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
		default :
			break;
	}	
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
	if(map_data[x][y].disp_flg != MAP_FLAG){
			open_0( map_data, x, y);
	}
}

// 0が続く限りマップを開き続ける
void open_0(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int x, int y){
	if(map_data[x][y].disp_flg != MAP_OPEN && map_data[x][y].data != MAP_WALL){
		map_data[x][y].disp_flg = MAP_OPEN;
		if(map_data[x][y].data == MAP_NONE){
			open_0(map_data, x, y-1);
			open_0(map_data, x-1, y);
			open_0(map_data, x+1, y);
			open_0(map_data, x, y+1);
			open_0(map_data, x-1, y-1);
			open_0(map_data, x+1, y+1);
			open_0(map_data, x-1, y+1);
			open_0(map_data, x+1, y-1);
		}
	}
}

// ゲームが終了した時に表示する
void disp_bom(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int x, int y){
	int i,j;
	for(j=1; j<y+2; j++){
		for(i=1; i<x+2; i++){
			if(map_data[i][j].data == MAP_BOM){
				attron(COLOR_PAIR(MAP_BOM));
				mvaddch(j,i*2,'*');
				}
		}
	}
}

//ゲームのクリアorゲームオーバー判定
int status_check(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL],int row_size, int col_size, int bom){
	int x,y;
	int gameover_flg = 0;
	int open_sum = 0;

	for(y=1; y < col_size+1; y++){
		for (x = 1; x < row_size+1; x++) {
			if(map_data[x][y].disp_flg == MAP_OPEN){
				open_sum++;
				if(map_data[x][y].data == MAP_BOM)
					return GAME_OVER;
			}
		}
	}

	if( (row_size * col_size)-bom == open_sum){
		return GAME_CLEAR;
	}
	return GAME_PLAY;
}

//ゲームオーバーでの処理
void game_over(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int x, int y){
	int start_y = y+2;
	int start_x = 0;

	int input = ' ';

	disp_game( map_data, x, y, 1,1);
	disp_bom( map_data, x,y);

	{
		attron(COLOR_PAIR(GAME_OVER));
		mvprintw( start_y+1, start_x, "       GGGGG         A        E         E   EEEEEEE      ");
		mvprintw( start_y+2, start_x, "      G     G       A A       EE       EE   E            ");
		mvprintw( start_y+3, start_x, "     G             A   A      E E     E E   E            ");
		mvprintw( start_y+4, start_x, "     G    GGGG     AAA A      E E     E E   EEEEEE       ");
		mvprintw( start_y+5, start_x, "     G       G    A     A     E  E   E  E   E            ");
		mvprintw( start_y+6, start_x, "      G     G     A     A     E   E E   E   E            ");
		mvprintw( start_y+7, start_x, "       GGGGG     A       A    E    E    E   EEEEEEE      ");
		mvprintw( start_y+8, start_x, "                                                         ");
		mvprintw( start_y+9, start_x, "       OOOOO     V        V     EEEEEEE    RRRRRRR       ");
		mvprintw( start_y+10, start_x,"      O     O    V        V     E          R      R      ");
		mvprintw( start_y+11, start_x,"     O       O    V      V      E          R     R       ");
		mvprintw( start_y+12, start_x,"     O       O     V    V       EEEEEE     RRRRRR        ");
		mvprintw( start_y+13, start_x,"     O       O     V    V       E          R     R       ");
		mvprintw( start_y+14, start_x,"      O     O       V  V        E          R      R      ");
		mvprintw( start_y+15, start_x,"       OOOOO         VV         EEEEEEE    R       R     ");

		attron(COLOR_PAIR(MAP_NONE));
		mvprintw( start_y+17, start_x+4," Please push : 'q'");
	}
	do{
		input = input_key();
	}while(input != 'q');
}
//クリアの処理
void game_clear(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int x, int y){
	int start_y = y+2;
	int start_x = 0;

	int input = ' ';

	disp_game( map_data, x, y, 1,1);
	disp_bom( map_data, x,y);

	attron(COLOR_PAIR(GAME_CLEAR));
	{			
		mvprintw( start_y+1, start_x, "       GGGGG         A        E         E   EEEEEEE      ");
		mvprintw( start_y+2, start_x, "      G     G       A A       EE       EE   E            ");
		mvprintw( start_y+3, start_x, "     G             A   A      E E     E E   E            ");
		mvprintw( start_y+4, start_x, "     G    GGGG     AAA A      E E     E E   EEEEEE       ");
		mvprintw( start_y+5, start_x, "     G       G    A     A     E  E   E  E   E            ");
		mvprintw( start_y+6, start_x, "      G     G     A     A     E   E E   E   E            ");
		mvprintw( start_y+7, start_x, "       GGGGG     A       A    E    E    E   EEEEEEE      ");
		mvprintw( start_y+8, start_x, "                                                         ");
		mvprintw( start_y+9, start_x, "        CCC   L        EEEEEEE      A      RRRRRRR       ");
		mvprintw( start_y+10, start_x,"       C   C  L        E           A A     R      R      ");
		mvprintw( start_y+11, start_x,"      C       L        E          A   A    R     R       ");
		mvprintw( start_y+12, start_x,"      C       L        EEEEEE     AAA A    RRRRRR        ");
		mvprintw( start_y+13, start_x,"      C       L        E         A     A   R     R       ");
		mvprintw( start_y+14, start_x,"       C   C  L        E         A     A   R      R      ");
		mvprintw( start_y+15, start_x,"        CCC   LLLLLLL  EEEEEEE  A       A  R       R     ");

		attron(COLOR_PAIR(MAP_NONE));
		mvprintw( start_y+17, start_x+4," Please push : 'q'");
	}
	do{
		input = input_key();
	}while(input != 'q');
}
