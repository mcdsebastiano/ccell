#include <stdio.h>
#include <errno.h>
#include "stringbuffer.h"
#include "table.h"
#include "terminal.h"

/*** @CLEANUP ROW/COL TRAVERSAL ***/
void draw_frame(short width, short height)
{
    StringBuffer sb = SB_NULL;

    sb_append(&sb, ENABLE_DEC, 4);

    for (short y = 1; y < height; ++y) {
        char row_buffer[8];
        int ret = sprintf_s(row_buffer, 8, CSI "%dd", y);
        sb_append(&sb, row_buffer, ret);
        for (short x = 1; x < width + 1; ++x) {
            char col_buffer[8];
            int ret = sprintf_s(col_buffer, 8, CSI "%dG", x);
            sb_append(&sb, col_buffer, ret);

            if (y == 1) {
                sb_append(&sb, "a", 1);

            } else if (y == height - 1) {
                sb_append(&sb, "a", 1);

            } else {

                if (x == 1) 
                    sb_append(&sb, "a", 1);

                 else if (x == width ) 
                    sb_append(&sb, "a", 1);

            }
        }
        sb_append(&sb, "\n", 1);
    }

    sb_append(&sb, DISABLE_DEC, 4);
    fwrite(sb.buffer, sb.length, 1, stdout);
    sb_free(&sb);
}

void table_create(Table* t, short width, short height)
{
    t->vp = (Viewport) {
        .top = 3,
        .bottom = height,
        .left = 5,
        .right = width
    };

    t->cursor = (Selection) {
        .left = 0,
        .top = 0,
        .right = 1,
        .bottom = 1
    };

    t->rows = height;
    t->cols = width;

    for (short y = 0; y < MAX_ROWS; ++y) {
        for (short x = 0; x < MAX_COLS; ++x) {
            Cell* cell = &t->grid[y][x];
            cell->x_pos = (x * CELLSIZE) + t->vp.left;
            cell->y_pos = y + t->vp.top;
        }
    }
}

void table_draw(Table* t)
{
    StringBuffer sb = SB_NULL;
    
    sb_append(&sb, GOTO_ROW(2), 4);
    sb_append(&sb, GOTO_COL(2), 4);
    for(int x = t->vp.left; x < t->vp.right; ++x) {
        
        char buffer[8];
        int ret = sprintf_s(buffer, 8, "%c", (x / CELLSIZE) + 64);
        
        if (x % CELLSIZE == 4) 
            sb_append(&sb, buffer, ret);
        else 
            sb_append(&sb, " ", 1); 
    }
    
    for(int y = t->vp.top; y < t->vp.bottom; ++y) {
        char row_buffer[8];
        int ret = sprintf_s(row_buffer, 8, CSI "%dd", y);
        sb_append(&sb, GOTO_COL(2), 4);
        sb_append(&sb, row_buffer, ret);
        
        char buffer[8];
        ret = sprintf_s(buffer, 8, "%3d\n", y - 2);

        sb_append(&sb, buffer , ret);
    }

    for (short row = t->vp.top; row < t->vp.bottom; ++row) {
        char row_buffer[8];
        int ret = sprintf_s(row_buffer, 8, CSI "%dd", row);
        sb_append(&sb, row_buffer, ret);
        for (short col = t->vp.left; col < t->vp.right; ++col) {
            char col_buffer[8];
            int ret = sprintf_s(col_buffer, 8, CSI "%dG", col);
            sb_append(&sb, col_buffer, ret);

            short x = (col - t->vp.left) / CELLSIZE;
            short y = row - t->vp.top;
            Cell cell = t->grid[y][x];

            // @Refactor,
            // @TODO: Blinking cursor for single cell,
            if (y >= t->cursor.top && y < t->cursor.bottom)
                if (x  >= t->cursor.left && x < t->cursor.right)
                    sb_append(&sb, COLOR(INVERTED), 4);


            if (cell.x_pos == col)
                sb_append(&sb, "|", 1);
            else
                sb_append(&sb, " ", 1);

            sb_append(&sb, COLOR(DEFAULT), 4);

        }
    }

    char status_message[8];
    sprintf_s(status_message, 8, "%c%d", t->cursor.left + 65, t->cursor.top + 1);
    print_status_message(t, status_message, &sb);
    
    short x = 0, y = 0;
    get_cursor_xy(t, &x, &y);
    
    char cursor_x[8];
    int ret = sprintf_s(cursor_x, 8, CSI "%dG", x);
    sb_append(&sb, cursor_x, ret);
    
    char cursor_y[8];
    ret = sprintf_s(cursor_y, 8, CSI "%dd", y);
    sb_append(&sb, cursor_y, ret);
    
    fwrite(sb.buffer, sb.length, 1, stdout);
    sb_free(&sb);
}

int within_vp_y(Table* t, short y)
{
    short y1 = t->vp.top + t->cursor.top + y;
    short y2 = t->vp.top + t->cursor.bottom + y;

    return y2 <= t->vp.bottom && y1 >= t->vp.top;
}

int within_vp_x(Table* t, short x)
{
    short x1 = t->vp.left + t->cursor.left + x;
    short x2 = t->vp.left + t->cursor.right + x;

    return x2 <= (t->vp.right / CELLSIZE) + t->vp.left &&  x1 >= t->vp.left;
}

void move_selection(Table* t, short x, short y)
{
    if (!within_vp_x(t, x))
        return;

    if (!within_vp_y(t, y))
        return;

    t->needs_update = 1;

    t->cursor.left += x;
    t->cursor.right += x;

    t->cursor.top += y;
    t->cursor.bottom += y;
}

void modify_selection(Table* t, short x, short y)
{
    if (!within_vp_x(t, x))
        return;

    if (!within_vp_y(t, y))
        return;

    t->needs_update = 1;

    if (y == 0) {
        if (x > 0)
            t->cursor.right += 1;
        else
            t->cursor.left -= 1;

        return;
    }

    if (x == 0) {
        if (y > 0)
            t->cursor.bottom += 1;
        else
            t->cursor.top -= 1;

        return;
    }

} 

// determines & stores the value which will adjust the cursors position/selection
void position_cursor(int keycode, short* x, short* y)
{
    switch (keycode) {
        case CTRL_ARROW_UP:
        case ARROW_UP:
            *x = 0;
            *y = -1;
            break;
        case CTRL_ARROW_DOWN:
        case ARROW_DOWN:
            *x = 0;
            *y = 1;
            break;
        case CTRL_ARROW_RIGHT:
        case ARROW_RIGHT:
            *x = 1;
            *y = 0;
            break;
        case CTRL_ARROW_LEFT:
        case ARROW_LEFT:
            *x = -1;
            *y = 0;
            break;
        default:
            return;
    }
}

void clear_selection(Table* t)
{
    t->cursor.right = t->cursor.left + 1;
    t->cursor.bottom = t->cursor.top + 1;
    t->needs_update = 1;
}

// @Refactor
// @Efficiency (buffer)
int read_file(char* filename, char* buffer[MAX_ROWS][MAX_COLS])
{
    // @TODO: incomplete
    FILE* f;
    errno_t error;
    error = fopen_s(&f, filename, "r");
    if (error != 0 ) {
        // Error Reading file.
    }

    if (f) {
        error = fclose(f);
        if (error != 0) {
            // Error Closing file.
        }
    }
    return 0;
}

void print_status_message(Table* t, char* message, StringBuffer* sb)
{ 
    int ret;
    char row_buffer[8];
    ret = sprintf_s(row_buffer, 8, CSI "%dd", t->vp.bottom + 1);
    sb_append(sb, row_buffer, ret);
    
    
    char col_buffer[8];
    ret = sprintf_s(col_buffer, 8, CSI "%dG", 1); 
    sb_append(sb, col_buffer, ret);
    sb_append(sb, DELETE_LINE, 4);
    sb_append(sb, message, sizeof(message) - 1);
}

void get_cursor_xy(Table* t, short* x, short* y)
{
    *x = t->vp.left + (t->cursor.left * CELLSIZE);
    *y = t->cursor.top + t->vp.top;
}

int handle_input(char c)
{
    switch (c) {
        case CTRL_KEY('q'):
            return EXIT_KEY;
        case CTRL_KEY('d'):
            return DESELECT_KEY;
        case  CTRL_KEY('e'):
            return COMMAND_KEY;
        case CTRL_KEY('g'):
            return GOTO_KEY;
        case  CTRL_KEY('s'):
            return SAVE_SESSION_KEY;
        case  CTRL_KEY('o'):
            return OPEN_FILE_KEY;
        case  CTRL_KEY('n'):
            return NEW_SESSION_KEY;
        case '\x1b':
            char seq[6];
            read_sequence(seq, 1);

            if (seq[0] == '[') {
                if (is_arrow_sequence(seq)) {
                    switch (seq[0]) {
                        case 'A':
                            return ARROW_UP;
                        case 'B':
                            return ARROW_DOWN;
                        case 'C':
                            return ARROW_RIGHT;
                        case 'D':
                            return ARROW_LEFT;
                        default:
                            break;
                    }
                } else if (is_ctrl_arrow_sequence(seq)) {
                    switch (seq[2]) {
                        case 'A':
                            return CTRL_ARROW_UP;
                        case 'B':
                            return CTRL_ARROW_DOWN;
                        case 'C':
                            return CTRL_ARROW_RIGHT;
                        case 'D':
                            return CTRL_ARROW_LEFT;
                        default:
                            break;
                    }
                }
            }
            break;
        default:
            return c;
    }
    return 0;
}
