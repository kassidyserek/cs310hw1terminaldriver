
#include "terminal.h"

// text columns
#define VGA_WIDTH 80
// text rows
#define VGA_HEIGHT 25
//starting address
#define VGA_MEMORY 0xB8000

// tracks the current column
static int x = 0;
//tracks the current row
static int y = 0;

// pointer to VGA video memory
static unsigned short *video = (unsigned short *) VGA_MEMORY;

void putc(int data) {
    // if the character is a new line, move cursor to start of next line
    if (data == '\n') {
        // reset column to beginning
        x = 0;
        // move down one row
        y++;
    } else {
        // calculate index into video memory
        int index = y * VGA_WIDTH + x;
        // 0x07 is light grey foreground on black background
        video[index] = (unsigned short)(data | 0x0700);
        // move cursor to next column
        x++;
    }

    // if reach the end of a line, wrap to next row
    if (x >= VGA_WIDTH) {
        // start at column 0
        x = 0;
        // move down one row
        y++;
    }

    // if move past bottom of screen, scroll
    if (y >= VGA_HEIGHT) {
        // scroll screen up by copying each row into the one above
        for (int row = 1; row < VGA_HEIGHT; row++) {
            for (int col = 0; col < VGA_WIDTH; col++) {
                video[(row - 1) * VGA_WIDTH + col] =
                    video[row * VGA_WIDTH + col];
            }
        }

        // clear last line - fill it with spaces
        for (int col = 0; col < VGA_WIDTH; col++) {
            video[(VGA_HEIGHT - 1) * VGA_WIDTH + col] =
                (unsigned short)(' ' | 0x0700);
        }

        // set cursor to last visible row
        y = VGA_HEIGHT - 1;
    }
}

void puts(const char *str) {
    // loop through string
    while (*str) {
        // print current character and move to next one
        putc(*str++);
    }
}
