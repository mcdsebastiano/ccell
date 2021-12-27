#ifndef TABLE_H
#define TABLE_H

#define MAX_VALUE 192
#define MAX_ROWS 255
#define MAX_COLS 75
#define CELLSIZE 8

typedef unsigned char u8;

enum input_sequences {
    EXIT_KEY = -1,
    ENTER_KEY = 13,
    SPACE_KEY = 32,
    BACKSPACE = 127,
    ARROW_UP = 1000,
    ARROW_DOWN,
    ARROW_LEFT,
    ARROW_RIGHT,
    DESELECT_KEY,
    OPEN_FILE_KEY,
    SAVE_SESSION_KEY,
    NEW_SESSION_KEY,
    DELETE_KEY,
    COMMAND_KEY, 
    GOTO_KEY,
    CTRL_ARROW_UP = 2000,
    CTRL_ARROW_DOWN,
    CTRL_ARROW_LEFT,
    CTRL_ARROW_RIGHT
};

typedef struct {
    StringBuffer value;
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
    u8 needs_update;
    Viewport vp;
    short rows;
    short cols;
} Table; 

void clear_selection(Table* t);
void draw_frame(short width, short height);
int within_vp_x(Table* t, short x);
int within_vp_y(Table* t, short y);
void table_setup(Table* t);
void table_draw(Table* t);
void get_cursor_xy(Table* t, short *x, short *y);
void move_selection(Table* t, short x, short y);
void modify_selection(Table *t, short x, short y);
void position_cursor(int keycode, short* x, short* y);
void print_status_message(Table *t, char* message, StringBuffer* sb);
int handle_input(char c);
int read_file(char* filename, char* buffer[MAX_ROWS][MAX_COLS]);
void delete_char(StringBuffer* sb, int at);

#endif