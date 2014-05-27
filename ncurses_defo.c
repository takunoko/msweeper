// cursesの基本的な関数
#include	<curses.h>

//cursesの初期化
void init_ncurses(){
	//ncurses環境の初期化
	if(initscr() == NULL){
		fprintf(stderr, "initscr failure\n");
		exit(EXIT_FAILURE);
	}
}
//curses終了処理
void end_ncurses(){
	if(endwin() == ERR){
		fprintf(stderr, "endwin failure");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
