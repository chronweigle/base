#ifndef RED_ECLIPSE_JSON_H
#define RED_ECLIPSE_JSON_H
#include <support/jsmn.h>


//These functions are taken from the jsmn examples


static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return -1;
}


/* Function realloc_it() is a wrapper function for standart realloc()
 * with one difference - it frees old memory pointer in case of realloc
 * failure. Thus, DO NOT use old data pointer in anyway after call to
 * realloc_it(). If your code has some kind of fallback algorithm if
 * memory can't be re-allocated - use standart realloc() instead.
 */
static inline void *realloc_it(void *ptrmem, size_t size) {
    void *p = realloc(ptrmem, size);
    if (!p)  {
        free (ptrmem);
        fprintf(stderr, "realloc(): errno=%d\n", errno);
    }
    return p;
}
#endif //RED_ECLIPSE_JSON_H