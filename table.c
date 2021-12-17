#include "table.h"
#include "terminal.h"

#include <stdio.h>
#include <string.h>

void draw_frame(short left, short top, short right, short bottom)
{
    enable_dec();
    for (short y = top; y < bottom; ++y) {
        goto_row(y + 1);
        for (short x = left; x < right; ++x) {
            goto_col(x + 1);

            if (y == 0) {

                // TOP
                if (x == 0)
                    fwrite("a", 1, 1, stdout);
                else if (x == right - 1)
                    fwrite("a", 1, 1, stdout);
                else
                    fwrite("a", 1, 1, stdout);

            } else if (y == bottom - 1) {

                // BOTTOM
                if (x == 0)
                    fwrite("a", 1, 1, stdout);
                else if (x == right - 1)
                    fwrite("a", 1, 1, stdout);
                else
                    fwrite("a", 1, 1, stdout);

            } else {

                if (x == 0) {
                    fwrite("a", 1, 1, stdout);
                } else if (x == 1) {
                    
                    if (y > 1) {
                        char buffer[8];
                        int ret = sprintf_s(buffer, 8, "%d", y - 1);
                        fwrite(buffer, ret, 1, stdout);
                    }
                    
                } else if (x == right - 1) {
                    fwrite("a", 1, 1, stdout);
                } else if (x > 0 && x < right - 1 && x % CELLSIZE == 0) {
                    if (y == 1) {
                        char buffer[8];
                        int ret = sprintf_s(buffer, 8, "%c", (x / CELLSIZE) + 64);
                        fwrite(buffer, ret, 1, stdout);
                    }
                }
                
            }
        }
    }
    disable_dec();
}

void table_create(Table* t)
{
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
    for (short y = t->vp.top; y < t->vp.bottom; ++y) {
        goto_row(y);
        for (short x = t->vp.left; x < t->vp.right; ++x) {
            goto_col(x);

            short xx = (x - t->vp.left) / CELLSIZE;
            short yy = y - t->vp.top;
            Cell cell = t->grid[yy][xx];

            // @Refactor,
            // @TODO: Blinking cursor for single cell,
            if (yy >= t->cursor.top && yy < t->cursor.bottom)
                if (xx  >= t->cursor.left && xx < t->cursor.right)
                    cell.color = INVERTED;

            set_color(cell.color);

            // @Refactor
            if (cell.x_pos == x)
                fwrite("|", 1, 1, stdout);
            else
                fwrite(" ", 1, 1, stdout);
            
            set_color(DEFAULT);


        }
    }

    char status_message[8];
    sprintf_s(status_message, 8, "%c%d", t->cursor.left + 65, t->cursor.top + 1);
    print_status_message(t, status_message);

    short x = 0, y = 0;
    get_cursor_xy(t, &x, &y);

    goto_position(x + 1, y);
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
void position_cursor(char key, short* x, short* y)
{
    switch (key) {
        case 'A':
            *x = 0;
            *y = -1;
            break;
        case 'B':
            *x = 0;
            *y = 1;
            break;
        case 'C':
            *x = 1;
            *y = 0;
            break;
        case 'D':
            *x = -1;
            *y = 0;
            break;
    }
}

void clear_selection(Table* t)
{
    t->cursor.right = t->cursor.left + 1;
    t->cursor.bottom = t->cursor.top + 1;
}

// @Refactor
// @Efficiency (buffer)
int read_file(char* filename, char* buffer[MAX_ROWS][MAX_COLS])
{
    // @TODO: incomplete
    FILE* f = fopen(filename, "r");
    fclose(f);
    return 0;
}

void print_status_message(Table* t, char* message)
{
    goto_position(1, t->vp.bottom + 1);
    delete_line();
    fwrite(message, sizeof(message) - 1, 1, stdout);
}

void get_cursor_xy(Table* t, short* x, short* y)
{
    *x = t->vp.left + (t->cursor.left * CELLSIZE);
    *y = t->cursor.top + t->vp.top;
}