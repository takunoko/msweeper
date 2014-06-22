#include <stdio.h>
#include <string.h>
/* curses ライブラリを使う際に必要なヘッダ */
#include	<curses.h>

#include "ms_game.h"
#include "my_curses.h"
#include "init_game.h"

int main(void){
	int game_status = GAME_PLAY;
	int input_char = ' ';

	init_curses();
	init_app();
	game_main();
	end_curses();

	return 0;
}
