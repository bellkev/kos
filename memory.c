#include "memory.h"

/*

Use masks like:              000100...
To set old values like:      110010...
To new values like:          000110...

Set:

Old  Mask New
1    1    1
0    1    1
1    0    1
0    0    0

Unset:

Old  Mask New
1    0    0
0    0    0
1    1    1
0    1    0

*/


int bitmap[BITMAP_WORD_COUNT];

void bm_reset() {
    for (int i = 0; i < BITMAP_WORD_COUNT; i++) {
        bitmap[i] = 0;
    }
}

void bm_set(int page, int set) {
    int word_index = page / WORD_SIZE;
    int offset = page % WORD_SIZE;
    if (set) {
        int mask = 1 << offset;
        bitmap[word_index] |= mask;
    } else {
        int mask = (1 << offset) ^ 0xffffffff;
        bitmap[word_index] &= mask;
    }
}

int bm_get(int page) {
    int word_index = page / WORD_SIZE;
    int offset = page % WORD_SIZE;
    return !!(bitmap[word_index] & (1 << offset));
}
