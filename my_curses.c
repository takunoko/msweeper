#include <stdlib.h>
// cursesの基本的な関数
#include <curses.h>

//cursesの初期化
void init_curses(){
	//ncurses環境の初期化
	if(initscr() == NULL){
		fprintf(stderr, "initscr failure\n");
		exit(EXIT_FAILURE);
	}
	//色が使えるかどうか
	if(has_colors() != true){
		fprintf(stderr, "cant use color\n");
		exit(EXIT_FAILURE);	
	}
	start_color();	//色が使用できるように設定

	echo();	//入力された文字を画面に表示しない
	cbreak();	//Enterナシでの入力
	nonl();
	leaveok(stdscr, FALSE); //カーソル位置を表示
	intrflush(stdscr, FALSE); //キーボードからの割り込みがあった場合、ウインドウの内容をflashするか
	keypad(stdscr, TRUE); //FALSE指定でファンクションキーをエスケープシーケンスのまま取り出す
}

//curses終了処理
void end_curses(){
	if(endwin() == ERR){
		fprintf(stderr, "endwin failure");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
