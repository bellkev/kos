#include "serial.h"

void log(char * s) {
    for (int i = 0;; i++) {
        if (s[i] == 0) {
            break;
        }
        write_serial(s[i]);
    }
    write_serial(10);
}

void log_hex(int d) {
    // TODO: Maybe compose this and string logging...

    int ascii_codes[] = {0x30, 0x31, 0x32, 0x33,
                         0x34, 0x35, 0x36, 0x37,
                         0x38, 0x39, 0x41, 0x42,
                         0x43, 0x44, 0x45, 0x46};

    write_serial('0');
    write_serial('x');

    for (int i = 28; i >= 0; i-=4) {
        write_serial(ascii_codes[(d >> i) & 0xf]);
    }

    write_serial(10);
}
