#ifndef TABLE_H
#define TABLE_H

#define MAX_ROWS 255
#define MAX_COLS 50

#define CELLSIZE 8
#define MAX_VALUE 192

typedef unsigned char u8;

enum input_sequences {
    ARROW_RIGHT,
    ARROW_LEFT,
    ARROW_DOWN,
    ARROW_UP
};

typedef struct string_t {
    char* buffer;
    u8 length;
} String;

typedef struct {
    char value[CELLSIZE];
    u8 color;
    short x_pos;
    short y_pos;
} Cell;

typedef struct {
    short bottom;
    short right;
    short left;
    short top;    
} Selection, Viewport;

typedef struct table_t {
    Cell grid[MAX_ROWS][MAX_COLS];
    Selection cursor;
    Viewport vp;
    short rows;
    short cols;
} Table; 

void clear_viewport();
void clear_selection(Table* t);
void draw_frame(short left, short top, short right, short bottom);
int within_vp_x(Table* t, short x);
int within_vp_y(Table* t, short y);
void table_setup(Table* t);
void table_draw(Table* t);
void get_cursor_xy(Table* t, short *x, short *y);
void move_selection(Table* t, short x, short y);
void modify_selection(Table *t, short x, short y);
void position_cursor(char c, short* x, short* y);
void print_status_message(Table *t, char* message);

int read_file(char* filename, char* buffer[MAX_ROWS][MAX_COLS]);

#endif