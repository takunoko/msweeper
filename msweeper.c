#include <stdio.h>
#include <string.h>
/* curses ライブラリを使う際に必要なヘッダ */
#include	<curses.h>

#include "ms_game.h"
#include "my_curses.h"
#include "init_game.h"

int main(void){
	int game_status = 0;
	int input_char = ' ';

	init_curses();
	init_app();

	while(input_char != 'q'){
		game_status = game_loop();

		mvprintw( 30, 0 ,"GEME END? :q ");
		input_char = input_key();
	}

	end_curses();

	return 0;
}
