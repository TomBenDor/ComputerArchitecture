#include "ex1.h"


/**
 * In a little endian machine, the number 0x1 will be represented as followed:
 * 00000001 00000000 00000000 00000000
 * So when treated as a character pointer, &x would point to 0x1 (00000001).
 */
int is_big_endian() {
    // Saving the result in a cache.
    static int result = -1;
    if (result != -1) {
        return result;
    }

    int i = 1;
    char *p = (char *) &i;
    
    result = (*p != 0x1); // *p == 0x1 if and only if we're on a little endian machine.

    return result;
}

/**
 * By dividing our unsigned longs into two parts using an int pointer,
 * it is possible to preform a merge at just one step.
 */
unsigned long merge_bytes(unsigned long x, unsigned long int y) {
    int *px = (int *) &x, *py = (int *) &y;

    if (is_big_endian()) {
        // Changing x's last 4 bytes to y's last 4 bytes.
        px[1] = py[1];
    } else {
        // Changing x's first 4 bytes to y's first 4 bytes.
        px[0] = py[0];
    }

    return x;
}

/**
 * Replacing the i'th byte with b depends on its memory address:
 * In a big-endian machine, it'll be simply the i'th memory address.
 * In a little-endian machine, it'll be the opposite (sizeof(x) - i - 1).
 */
unsigned long put_byte(unsigned long x, unsigned char b, int i) {
    char *px = (char *) &x;

    if (is_big_endian()) {
        px[i] = b;
    } else {
        px[sizeof(x) - i - 1] = b;
    }

    return x;
}
