#include "terminal.h"
#include "table.h"
#include <stdio.h>

// int handle_input(Table* t, char c);

int main(int argc, char** argv)
{
    if (enable_vt() == 0) {
        printf("Unable to enter VT processing mode. Quitting.\n");
        return -1;
    }

    enable_alternate_buffer();
    hide_cursor();

    short width = 0, height = 0;
    get_screen_size(&width, &height);
    draw_frame(0, 0, width, height - 1);

    Table t;
    t.vp = (Viewport) {
        .top = 3,
        .bottom = height - 1,
        .left = 5,
        .right = width
    };

    t.cursor = (Selection) {
        .left = 0,
        .top = 0,
        .right = 1,
        .bottom = 1
    };

    table_create(&t);
    table_draw(&t);

    char c = '\0';
    while (fread(&c, 1, 1, stdin) != -1) {
        if (handle_input(&t, c) == -1) {
            break;
        }
    }

    display_cursor();
    disable_alternate_buffer();
    clear_screen();

    return 0;
}

int handle_input(Table* t, char c)
{

    short x = 0, y = 0;
    u8 redraw = 0;
    if (c == CTRL_KEY('q')) {
        return -1;
    } else if (c == CTRL_KEY('d')) {
        clear_selection(t);
        redraw = 1;
    } else if (c == CTRL_KEY('e')) {

    } else if (c == CTRL_KEY('[')) {

    } else if (c == CTRL_KEY(']')) {

    } else if (c == CTRL_KEY('o')) {

    } else if (c == CTRL_KEY('n')) {
        
    }
    if (c == '\x1b') {
        char seq[8];
        read_sequence(seq, 2);
        if (seq[0] == '[') {

            if (is_arrow_sequence(seq)) {
                position_cursor(seq[1], &x, &y);
                move_selection(t, x, y);
                redraw = 1;
            }

            if (seq[1] == '1') {
                read_sequence(seq, 3);
                if (is_ctrl_arrow_sequence(seq)) {
                    position_cursor(seq[2], &x, &y);
                    modify_selection(t, x, y);
                    redraw = 1;
                }
            }

        }
    } else {
        write_line(&c, 1);
    }

    if (redraw) {
        table_draw(t);
    }

    return 0;
}
