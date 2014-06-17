#define SELECT_SIZE 3

void init_app(void);
void init_map(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int*, int*, int*, int*, int*);
void create_bom(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int, int, int);
void create_map_num(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int, int);
int get_risk(MAP map_data[MAP_MAX_ROW][MAP_MAX_COL], int, int, int, int);
void init_char_color(void);
