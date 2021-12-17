#ifndef TERMINAL_H
#define TERMINAL_H

#include <windows.h>
#include <winioctl.h>

#define CTRL_KEY(k) ((k) & 0x1f)

#define ESC "\x1b"
#define CSI "\x1b["

#define DEFAULT 0
#define INVERTED 7

typedef unsigned char u8;
void die();
void get_screen_size(short* width, short* height);
void set_scrolling_region(short t, short b);
void goto_position(short x, short y);
void goto_col(short col);
void goto_row(short row);
void clear_screen();
void enable_dec();
void disable_dec();
void disable_cursor();
void enable_cursor();
void delete_line();
void delete_lines(short count);
void set_color(u8 color) ;
int is_ctrl_arrow_sequence(char* s);
int is_arrow_sequence(char* s);
void enable_alternate_buffer();
void disable_alternate_buffer();
int enable_vt(void);
void write_line(char* message, short size);
void read_sequence(char* sequence, short count);


#endif