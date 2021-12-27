#include "stringbuffer.h"
#include "terminal.h"
#include "table.h"
#include <stdio.h>

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;
    if (enable_vt() == 0) {
        printf("Unable to enter VT processing mode. Quitting.\n");
        return -1;
    }

    enable_alternate_buffer();
    hide_cursor();

    short width = 0, height = 0;
    get_screen_size(&width, &height);
    draw_frame(width, height);
    
    Table t;
    table_create(&t, width, height - 1);
    table_draw(&t);

    u8 typing;
    u8 exit = 0;
    char c = '\0';
    int keycode = 0;
    typing = 0;
    short x = 0, y = 0;
    while (read_sequence(&c, 1)) {

        //@Refactor
       
        t.needs_update = 0;
        keycode = handle_input(c);

        //@Cleanup: lots of loops!
        switch (keycode) {
            
            case BACKSPACE:
                if (typing == 0) {
                    delete_selection(&t);
                } else {
                    for (int y = t.cursor.top; y < t.cursor.bottom; ++y) {
                        for (int x = t.cursor.left; x < t.cursor.right; ++x) {
                            Cell* cell = &t.grid[y][x];
                            delete_char(&cell->value, cell->value.length);
                        }
                    }
                }
                t.needs_update = 1;
                break;
            case DELETE_KEY:
            //@TODO
            case SAVE_SESSION_KEY:
                //@TODO
                break;
            case OPEN_FILE_KEY:
                //@TODO
                break;
            case NEW_SESSION_KEY:
                //@TODO
                break;
            case COMMAND_KEY:
                //@TODO
                break;
            case GOTO_KEY:
                //@TODO
                break;
            case EXIT_KEY:
                exit = 1;
                break;
            case DESELECT_KEY:
                clear_selection(&t);
                break;
            case ARROW_UP:
            case ARROW_DOWN:
            case ARROW_LEFT:
            case ARROW_RIGHT:
                position_cursor(keycode, &x, &y);
                move_selection(&t, x, y);
                typing = 0;
                break;
            case CTRL_ARROW_UP:
            case CTRL_ARROW_DOWN:
            case CTRL_ARROW_LEFT:
            case CTRL_ARROW_RIGHT:
                position_cursor(keycode, &x, &y);
                modify_selection(&t, x, y);
                typing = 0;
                break;
            case ENTER_KEY:
                break;
            case SPACE_KEY:
            default:
            
                typing = 1;

                for (int y = t.cursor.top; y < t.cursor.bottom; ++y) {
                    for (int x = t.cursor.left; x < t.cursor.right; ++x) {
                        Cell* cell = &t.grid[y][x];
                        sb_append(&cell->value, &c, 1);
                    }
                }
                
                t.needs_update = 1;
                break;
        }

        if (exit)
            break;

        if (t.needs_update)
            table_draw(&t);

    }

    display_cursor();
    disable_alternate_buffer();
    clear_screen();

    return 0;
}
