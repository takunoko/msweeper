// cursesの基本的な関数
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>

//cursesの初期化
void init_curses(){
	//ncurses環境の初期化
	if(initscr() == NULL){
		fprintf(stderr, "initscr failure\n");
		exit(EXIT_FAILURE);
	}
	noecho();	//入力された文字を画面に表示しない
	nonl();
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
