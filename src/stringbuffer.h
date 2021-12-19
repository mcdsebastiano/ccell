#ifndef STRINGBUFFER_H
#define STRINGBUFFER_H

#define _STR_(s) #s
#define STR(s) _STR_(s)

#define SB_NULL {NULL,0}
#define SB_NEW(str_lit)             \
(StringBuffer) {                    \
    .length = sizeof(str_lit) - 1,  \
    .buffer = str_lit,              \
}                                   \

typedef struct string_t {
    char* buffer;
    int length;
} StringBuffer;

void sb_append(StringBuffer* sb, const char* s, int length);
void sb_free(StringBuffer *sb);

#endif