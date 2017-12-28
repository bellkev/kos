#include <assert.h>
#include <stdio.h>
#include "memory.h"

void test_bm_reset() {
    bm_set(0,1);
    bm_set(5,1);
    bm_set(500,1);
    bm_reset();
    for (int i; i < BITMAP_SIZE; i++) {
        assert(bm_get(i) == 0);
    }
}

void test_bm_set() {
    bm_reset();
    bm_set(10,1);
    assert(bm_get(10) == 1);
    bm_set(500,1);
    assert(bm_get(500) == 1);
}

void test_unsetting() {
    bm_reset();
    bm_set(10,1);
    bm_set(11,1);
    assert(bm_get(10) == 1);
    assert(bm_get(11) == 1);
    bm_set(10,0);
    assert(bm_get(10) == 0);
    assert(bm_get(11) == 1);
}
