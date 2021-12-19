#include <stdio.h>
#include <windows.h>
#include <winioctl.h>

#include "terminal.h"

// Die 
void die(const char* s)
{
    clear_screen();
    goto_position(1,1);
    perror(s);
    exit(1);
}

// Enable VT Mode
int enable_vt(void)
{
    // @Refactor
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) {
        return 0;
    }

    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    if (hIn == INVALID_HANDLE_VALUE) {
        return 0;
    }

    DWORD dwOriginalOutMode = 0;
    DWORD dwOriginalInMode = 0;

    if (!GetConsoleMode(hOut, &dwOriginalOutMode)) {
        return 0;
    }

    if (!GetConsoleMode(hIn, &dwOriginalInMode)) {
        return 0;
    }

    DWORD dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
    DWORD dwRequestedInModes = ENABLE_VIRTUAL_TERMINAL_INPUT | !ENABLE_LINE_INPUT;
    DWORD dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
    if (!SetConsoleMode(hOut, dwOutMode)) {
        // we failed to set both modes, try to step down mode gracefully.
        dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
        if (!SetConsoleMode(hOut, dwOutMode)) {
            // Failed to set any VT mode, can't do anything here.
            return 0;
        }
    }
    DWORD dwInMode = dwRequestedInModes;
    if (!SetConsoleMode(hIn, dwInMode)) {
        // Failed to set VT input mode, can't do anything here.
        return 0;
    }

    return 1;
}

// Arrow Sequence
int is_arrow_sequence(char* s) {
    read_sequence(s, 1);
    return  s[0] >= 'A' && s[0] <= 'D';
}

// Control Arrow Sequence
int is_ctrl_arrow_sequence(char* s)
{
    if(s[0] == '1') {
        read_sequence(s, 3);
        s[4] = '\0';
        return s[0] == ';' && s[1] == '5' && s[2] >= 'A' && s[2] <= 'D';
    } 
    return 0;
}

// Clear Screen
void clear_screen()
{
    // @TODO: clear screen as per the MSDN
    fwrite(CSI "2J", 4, 1, stdout);
}

// Set Color
void set_color(u8 color)
{
    char buffer[8];
    int ret = sprintf_s(buffer, 8, CSI "%dm", color);
    fwrite(buffer, ret, 1, stdout);
}

// Enable Alternate Buffer
void enable_alternate_buffer()
{
    fwrite(CSI "?1049h", 8, 1, stdout);
}

// Disable Alternate Buffer
void disable_alternate_buffer()
{
    fwrite(CSI "?1049l", 8, 1, stdout);
}

// Enable Designate Drawing 
void enable_dec()
{
    fwrite(ESC "(0", 4, 1, stdout);
}

//Disable Designate Drawing
void disable_dec()
{
    fwrite(ESC "(B", 4, 1, stdout);
}

// Get Screen Size
void get_screen_size(short* width, short* height)
{
    HANDLE hAlt  = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO buffer_info;
    GetConsoleScreenBufferInfo(hAlt, &buffer_info);

    *width = buffer_info.dwSize.X;
    *height = buffer_info.dwSize.Y;
}

// Set Scrolling Region
void set_scrolling_region(short t, short b)
{
    char buffer[8];
    int ret = sprintf_s(buffer, 8, CSI "%d;%dr", t, b);
    fwrite(buffer, ret, 1, stdout);
}

// Goto Position
void goto_position(short x, short y)
{
    goto_row(y);
    goto_col(x);
}

// Goto Column
void goto_col(short col)
{
    char buffer[8];
    int ret = sprintf_s(buffer, 8, CSI "%dG", col);
    fwrite(buffer, ret, 1, stdout);
}

// Goto Row
void goto_row(short row)
{
    char buffer[8];
    int ret = sprintf_s(buffer, 8, CSI "%dd", row);
    fwrite(buffer, ret, 1, stdout);
}

// Hide Cursor
void hide_cursor(void)
{
    fwrite(CSI "?25l", 6, 1, stdout);
}

// Display Cursor
void display_cursor(void)
{
    fwrite(CSI "?25h", 6, 1, stdout);
}

void delete_line(void) 
{
    fwrite(CSI "1M", 4, 1, stdout);
}

void delete_lines(short count)
{
    char buffer[8];
    int ret = sprintf_s(buffer, 8, CSI "%dM", count);
    fwrite(buffer, ret, 1, stdout);
}

void write_line(char* message)
{
    fwrite(message, sizeof(message) - 1, 1, stdout);
}

int read_sequence(char* sequence, short count)
{
    int i = 0;
    for (i = 0; i < count; ++i) {
        if(fread(&sequence[i],1,1,stdin) != 1) {
            break;
        }
    }
    
    return i == count;
}
