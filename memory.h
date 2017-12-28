#define WORD_SIZE 32
#define PAGE_SIZE 1<<22         /* 4MB */
#define INT_MAX 0xFFFFFFFF      /* Max address */
#define BITMAP_SIZE 1024 /* INT_MAX + 1 (addressable word count) / PAGE_SIZE */
#define BITMAP_WORD_COUNT BITMAP_SIZE / WORD_SIZE /* BITMAP_SIZE / WORD_SIZE */

void bm_reset();
void bm_set(int page, int set);
int bm_get(int page);
