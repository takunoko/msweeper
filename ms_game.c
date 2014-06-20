#include <curses.h>

#include "msweeper.h"
#include "ms_game.h"
#include "init_game.h"

void game_main(){
	int input_char = ' ';

	while(input_char != 'q'){
		//このループにもうちょい意味を持たせたい
		game_loop();
	}
}

// 1回のゲームは、この中でループ
int game_loop(){
	int game_status = GAME_PLAY;
	int input_char = ' ';

	MAP map_data[(MAP_MAX_ROW)][(MAP_MAX_COL)];
	int map_row_size, map_col_size, bom;
	int pos_x, pos_y;

	init_map(map_data, &map_row_size, &map_col_size, &bom, &pos_x, &pos_y);

	while(game_status == GAME_PLAY){
		disp_game( map_data, map_row_size, map_col_size, pos_x, pos_y);
		input_char = input_key();
		move_pos( map_data,  input_char, map_row_size, map_col_size, &pos_x, &pos_y);
		game_status = status_check( map_data, map_row_size, map_col_size, bom);

		switch (game_status){
			case GAME_OVER:
				game_over( map_data, map_row_size, map_col_size);
				init_map(map_data, &map_row_size, &map_col_size, &bom, &pos_x, &pos_y);
				break;
			case GAME_CLEAR:
				game_clear( map_data, map_row_size, map_col_size);
				init_map(map_data, &map_row_size, &map_col_size, &bom, &pos_x, &pos_y);
				break;
			default :
				break;
		}
	}

	return 0;
}

// ゲーム画面の描画
void disp_game(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int row_size, int col_size, int pos_x, int pos_y){
	int x,y;
	for(y=0; y<col_size+2; y++){
		for(x=0; x<row_size+2; x++){
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
	if(map_data[x][y].disp_flg != MAP_FLAG){
			open_0( map_data, x, y);
	}
}

// 0が続く限りマップを開き続ける
void open_0(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int x, int y){
	if(map_data[x][y].disp_flg != MAP_OPEN){
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

//ゲームオーバーでの処理
void game_over(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int x, int y){
	int i=0;
	clear();

	attron(COLOR_PAIR(GAME_OVER));
	mvprintw( 1,i,"       GGGGG         A        E         E   EEEEEEE             ");
	mvprintw( 2,i,"      G     G       A A       EE       EE   E                   ");
	mvprintw( 3,i,"     G             A   A      E E     E E   E                   ");
	mvprintw( 4,i,"     G    GGGG     AAA A      E E     E E   EEEEEE              ");
	mvprintw( 5,i,"     G       G    A     A     E  E   E  E   E                   ");
	mvprintw( 6,i,"      G     G     A     A     E   E E   E   E                   ");
	mvprintw( 7,i,"       GGGGG     A       A    E    E    E   EEEEEEE             ");
 	mvprintw( 8,i,"                                                                ");
	mvprintw( 9,i,"       OOOOO    V        V   EEEEEEE    RRRRRRR                 ");
	mvprintw(10,i,"      O     O   V        V   E          R      R                ");
	mvprintw(11,i,"     O       O   V      V    E          R     R                 ");
	mvprintw(12,i,"     O       O    V    V     EEEEEE     RRRRRR                  ");
	mvprintw(13,i,"     O       O    V    V     E          R     R                 ");
	mvprintw(14,i,"      O     O      V  V      E          R      R                ");
	mvprintw(15,i,"       OOOOO        VV       EEEEEEE    R       R               ");
	input_key();
}

//クリアの処理
void game_clear(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int x, int y){
	clear();
	int i=0;

	attron(COLOR_PAIR(GAME_CLEAR));
	{			
		mvprintw( 1,i,"       GGGGG         A        E         E   EEEEEEE             ");
		mvprintw( 2,i,"      G     G       A A       EE       EE   E                   ");
		mvprintw( 3,i,"     G             A   A      E E     E E   E                   ");
		mvprintw( 4,i,"     G    GGGG     AAA A      E E     E E   EEEEEE              ");
		mvprintw( 5,i,"     G       G    A     A     E  E   E  E   E                   ");
		mvprintw( 6,i,"      G     G     A     A     E   E E   E   E                   ");
		mvprintw( 7,i,"       GGGGG     A       A    E    E    E   EEEEEEE             ");
		mvprintw( 8,i,"                                                                ");
		mvprintw( 9,i,"        CCC   L        EEEEEEE      A      RRRRRRR              ");
		mvprintw(10,i,"       C   C  L        E           A A     R      R             ");
		mvprintw(11,i,"      C       L        E          A   A    R     R              ");
		mvprintw(12,i,"      C       L        EEEEEE     AAA A    RRRRRR               ");
		mvprintw(13,i,"      C       L        E         A     A   R     R              ");
		mvprintw(14,i,"       C   C  L        E         A     A   R      R             ");
		mvprintw(15,i,"        CCC   LLLLLLL  EEEEEEE  A       A  R       R            ");
	}
	input_key();
}

//ゲームのクリアorゲームオーバー判定
int status_check(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL],int row_size, int col_size, int bom){
	int x,y;
	int gameover_flg = 0;
	int open_sum = 0;

	for(y=1; y < row_size+1; y++){
		for (x = 1; x < col_size+1; x++) {
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
