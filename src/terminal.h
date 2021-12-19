#ifndef TERMINAL_H
#define TERMINAL_H

#define CTRL_KEY(k) ((k) & 0x1f)

#define ESC "\x1b"
#define CSI "\x1b["

#define DEFAULT 0
#define INVERTED 7

#define DELETE_LINE CSI "1M"
#define HIDE_CURSOR CSI "?25l"
#define DISPLAY_CURSOR CSI "?25h"
#define ENABLE_DEC ESC "(0"
#define DISABLE_DEC ESC "(B"
#define ENABLE_ALTERNATE_BUFFER CSI "?1049h"
#define DISABLE_ALTERNATE_BUFFER CSI "?1049l"
#define CLEAR_SCREEN CSI "2J"

#define COLOR(c) CSI STR(c) "m"
#define GOTO_ROW(y) CSI STR(y) "d"
#define GOTO_COL(x) CSI STR(x) "G"

typedef unsigned char u8;
void die(const char* s);
void get_screen_size(short* width, short* height);
void set_scrolling_region(short t, short b);
void goto_position(short x, short y);
void goto_col(short col);
void goto_row(short row);
void clear_screen();
void enable_dec(void);
void disable_dec(void);
void disable_cursor(void);
void enable_cursor(void);
void delete_line(void);
void delete_lines(short count);
void set_color(u8 color) ;
int is_ctrl_arrow_sequence(char* s);
int is_arrow_sequence(char* s);
void enable_alternate_buffer(void);
void disable_alternate_buffer(void);
int enable_vt(void);
void write_line(char* message);
int read_sequence(char* sequence, short count);

#endif
