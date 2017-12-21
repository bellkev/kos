#include "io.h"

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

/* Some colors */
#define FB_BLACK 0
#define FB_WHITE 15


int cursor = 0;

void fb_move_cursor()
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT,    ((cursor >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT,    cursor & 0x00FF);
}

char * fb = (char *) 0xC00B8000;

void fb_write_cell(char c, unsigned char fg, unsigned char bg) {
    fb[cursor * 2] = c;
    fb[cursor * 2 + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

void fb_write(char c) {
    fb_write_cell(c, FB_BLACK, FB_WHITE);
    cursor++;
    fb_move_cursor();
}
