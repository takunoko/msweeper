#define MAP_OPEN	100
#define MAP_CLOSE 200

#define MAP_MAX_ROW 82
#define MAP_MAX_COL 82

#define MAP_NONE 11
#define MAP_WALL 12
#define MAP_BOM	 13
#define MAP_FLAG 14
#define MAP_CLOSE_COL 19

#define GAME_OVER	21
#define GAME_CLEAR 22

typedef struct{
	int data;
	int disp_flg;
}MAP;

int game_loop(void);
void disp_game(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int, int, int, int);
void move_pos(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int, int, int, int *, int *);
int input_key(void);
void map_open(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int, int, int, int);
void open_0(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int, int);
