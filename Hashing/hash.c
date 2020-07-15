/* treat strings as base-256 integers */
/* with digits in the range 1 to 255 */

#define BASE (256)

size_t hashDivision(const char *s, size_t m) {
    unsigned const char *us;
    size_t h = 0;

    us = (unsigned const char *) s;

    while (*us != '\0') {
        h = (h * BASE + *us) % m;
        *us++;
    }

    return h;
}

#define MULTIPLIER (37)

size_t hashMultiplier(const char *s, size_t m) {
    unsigned const char *us;
    size_t h = 0;

    us = (unsigned const char *) s;

    while(*us != '\0') {
        h = MULTIPLIER * h + *us;
        us++;
    }

    return h;
}

#define BITS_PER_CHAR (8)
#define MAX_STRING_SIZE (128)
#define MAX_BITS (BITS_PER_CHAR * MAX_STRING_SIZE)

size_t universalHash(const char *s, size_t x[]) {
    size_t h;
    unsigned const char *us;
    int i;
    unsigned char c;
    int shift;

    us = (unsigned const char *) s;

    h = 0;

    for (i = 0; *us != 0, i < MAX_BITS; *us++) {
        c = *us;
        for (shift = 0; shift < BITS_PER_CHAR; shift++, i++) {
            if (c & 0x1) {
                h ^= x[i];
            }

            c >> 1;
        }
    }

    return h;
}