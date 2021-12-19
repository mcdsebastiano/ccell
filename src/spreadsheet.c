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
    
    u8 exit = 0;
    char c = '\0';
    int keycode = 0;
    short x = 0, y = 0;
    while (read_sequence(&c, 1)) {

        // @Refactor
        t.needs_update = 0;
        keycode = handle_input(c);

        switch (keycode) {
            case BACKSPACE:
                //@TODO
                break;
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
                break;
            case CTRL_ARROW_UP:
            case CTRL_ARROW_DOWN:
            case CTRL_ARROW_LEFT:
            case CTRL_ARROW_RIGHT:
                position_cursor(keycode, &x, &y);
                modify_selection(&t, x, y);
                break;
            default:
                // @TODO: append char at position & save string
                write_line(&c);
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
