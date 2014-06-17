#define MAP_OPEN	1
#define MAP_CLOSE 0

#define MAP_MAX_ROW 100
#define MAP_MAX_COL 100

#define MAP_NONE 10
#define MAP_WALL 20
#define MAP_BOM	 30
#define MAP_CLOSE_COL 40

typedef struct{
	int data;
	int disp_flg;
}MAP;
