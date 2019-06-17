#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct string {
    char *ptr;
    size_t len;
};


void _new_string(struct string *s, size_t len) {
    int l = (int)s->len;
    int n = 0;
    if (l > n) {
        s->ptr = (char *) realloc(s->ptr, l + len + 1);
        s->len = l + len + 1;
    } else {
        s->ptr = malloc(len + 1);
        s->len = len;
    }

    if (s->ptr == NULL) {
        fprintf(stderr, "Memory fail!");
    }
    s->ptr[s->len] = '\0';
}

struct string *concat(struct string *s1, struct string *s2) {
    size_t l = s1->len;
    _new_string(s1, s2->len);
    memcpy(s1->ptr+l, s2->ptr, s2->len);
    s1->len = l + s2->len;
    return s1;
}

struct string *newString(char* c) {
    struct string *s = malloc(sizeof *s);
    s->len = 0;
    _new_string(s, strlen(c));
    if(strlen(c) > 0) {
        memcpy(s->ptr, c, strlen(c));
    }
    return s;
}

char *getChar(struct string *s) {
    char *r = malloc(s->len+1);
    memcpy(r, s->ptr, s->len);
    r[s->len] = '\0';
    return r;
}
