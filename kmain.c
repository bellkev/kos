#define FB_BLACK 0
#define FB_WHITE 15

char * fb = (char *) 0x000B8000;

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg) {
    fb[i * 2] = c;
    fb[i * 2 + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

void hello() {
    char * message = "Hello, World!!!";
    for (int i = 0; i < 15; i++) {
        fb_write_cell(i, message[i], FB_BLACK, FB_WHITE);
    }
}
