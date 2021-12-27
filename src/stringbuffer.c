#include <stdlib.h>
#include <string.h>

#include "stringbuffer.h"

void sb_append(StringBuffer* sb, const char* s, int length)
{
    
    char *new = realloc(sb->buffer, sb->length + length);
    
    if (new == NULL)
        return;
    
    memcpy(&new[sb->length], s, length);
    sb->buffer = new;
    sb->length += length;
}

void sb_free(StringBuffer *sb)
{
    free(sb->buffer);
    sb->length = 0;
}
