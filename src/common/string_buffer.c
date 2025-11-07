
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#include "alloc.h"
#include "string_buffer.h"

string_t* create_string(const char* str) {

    string_t* ptr = _ALLOC_TYPE(string_t);
    ptr->cap = 1 << 3;
    ptr->len = 0;
    ptr->buffer = _ALLOC_ARRAY(char, ptr->cap);

    if(str != NULL)
        append_string(ptr, str);

    return ptr;
}

string_t* create_string_fmt(const char* fmt, ...) {

    va_list args;

    va_start(args, fmt);
    size_t len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    char* str = _ALLOC(len + 1);

    va_start(args, fmt);
    vsprintf(str, fmt, args);
    va_end(args);

    string_t* ptr = create_string(str);
    _FREE(str);

    return ptr;
}

void destroy_string(string_t* buf) {

    if(buf != NULL) {
        _FREE(buf->buffer);
        _FREE(buf);
    }
}

string_t* append_string(string_t* buf, const char* str) {

    int len = strlen(str);
    if(len + buf->len + 1 > buf->cap) {
        while(len + buf->len + 1 > buf->cap)
            buf->cap <<= 1;
        buf->buffer = _REALLOC_ARRAY(buf->buffer, char, buf->cap);
    }

    strcat(buf->buffer, str);
    buf->len = strlen(buf->buffer);

    return buf;
}

string_t* append_string_fmt(string_t* buf, const char* fmt, ...) {

    va_list args;

    va_start(args, fmt);
    size_t len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    char* str = _ALLOC(len + 1);

    va_start(args, fmt);
    vsprintf(str, fmt, args);
    va_end(args);

    append_string(buf, str);
    _FREE(str);

    return buf;
}

string_t* append_string_char(string_t* buf, int ch) {

    if(buf->len + 1 > buf->cap) {
        buf->cap <<= 1;
        buf->buffer = _REALLOC_ARRAY(buf->buffer, char, buf->cap);
    }

    buf->buffer[buf->len] = (char)ch;
    buf->len++;
    buf->buffer[buf->len] = '\0';

    return buf;
}

string_t* append_string_str(string_t* buf, string_t* str) {

    return append_string(buf, str->buffer);
}

void clear_string(string_t* buf) {

    buf->len = 0;
    buf->buffer[0] = 0;
}

int len_string(string_t* buf) {

    return buf->len;
}

int comp_string(string_t* buf1, string_t* buf2) {

    return strcmp(buf1->buffer, buf2->buffer);
}

int comp_string_str(string_t* buf1, const char* buf2) {

    return strcmp(buf1->buffer, buf2);
}

const char* raw_string(string_t* str) {

    if(str != NULL)
        return str->buffer;
    else
        return NULL;
}

string_t* strip_char(string_t* buf, int ch) {

    strip_space(buf);

    char* temp;
    while((temp = strrchr(buf->buffer, ch)) != NULL)
        memmove(temp, temp + 1, strlen(temp) + 1);

    buf->len = strlen(buf->buffer);

    return buf;
}

string_t* strip_space(string_t* buf) {

    int len;
    for(len = strlen(buf->buffer); (isspace(buf->buffer[len]) || buf->buffer[len] == 0) && len >= 0; len--)
        buf->buffer[len] = '\0';

    for(len = 0; buf->buffer[len] != '\0' && isspace(buf->buffer[len]); len++) {
    }

    memmove(&buf->buffer[0], &buf->buffer[len], strlen(&buf->buffer[len]) + 1);

    return buf;
}

string_t* upcase(string_t* buf) {

    for(int i = 0; buf->buffer[i] != '\0'; i++)
        buf->buffer[i] = toupper(buf->buffer[i]);

    return buf;
}

string_t* downcase(string_t* buf) {

    for(int i = 0; buf->buffer[i] != '\0'; i++)
        buf->buffer[i] = tolower(buf->buffer[i]);

    return buf;
}

struct conv_tab {
    int ch;
    const char* str;
};

static struct conv_tab tab[] = {
    { '~', "TILDE" },
    { '`', "BQUOTE" },
    { '!', "BANG" },
    { '@', "AT" },
    { '#', "POUND" },
    { '$', "DOLLAR" },
    { '%', "PERCENT" },
    { '^', "CARET" },
    { '&', "AMP" },
    { '*', "STAR" },
    { '(', "OPAREN" },
    { ')', "CPAREN" },
    { '-', "MINUS" },
    { '=', "EQUAL" },
    { '+', "PLUS" },
    { '[', "OSBRACE" },
    { '{', "OCBRACE" },
    { ']', "CSBRACE" },
    { '}', "CCBRACE" },
    { '\\', "BSLASH" },
    { '|', "BAR" },
    { ';', "SCOLON" },
    { ':', "COLON" },
    { '\'', "SQUOTE" },
    { '\"', "DQUOTE" },
    { ',', "COMMA" },
    { '<', "OPBRACE" },
    { '.', "DOT" },
    { '>', "CPBRACE" },
    { '/', "SLASH" },
    { '?', "QUESTION" },
    { 0, NULL }
};

static char* conv_char(int ch, char* buffer, int size) {

    for(int i = 0; tab[i].str != NULL; i++) {
        if(ch == tab[i].ch) {
            strncpy(buffer, tab[i].str, size);
            return buffer;
        }
    }

    buffer[0] = ch;
    buffer[1] = '\0';

    return buffer;
}

string_t* convert(string_t* str) {

    string_t* buf = create_string(NULL);
    char tmp[64];

    for(int i = 0; str->buffer[i] != '\0'; i++) {
        if(!ispunct(str->buffer[i]) || str->buffer[i] == '_')
            append_string_char(buf, toupper(str->buffer[i]));
        else {
            append_string(buf, conv_char(str->buffer[i], tmp, sizeof(tmp)));
            if(str->buffer[i + 1] != '\0')
                append_string_char(buf, '_');
        }
    }
    return buf;
}

string_t* copy_string(string_t* buf) {

    return create_string(buf->buffer);
}

void emit_string(FILE* fp, string_t* ptr) {

    fputs(ptr->buffer, fp);
}

void emit_string_fmt(FILE* fp, const char* fmt, ...) {

    va_list args;

    va_start(args, fmt);
    size_t len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    char* str = _ALLOC(len + 1);

    va_start(args, fmt);
    vsprintf(str, fmt, args);
    va_end(args);

    fputs(str, fp);
    _FREE(str);
}

#if 0
int main(void) {

    string_t* ptr = create_string("'   this isi the string   \"");

    strip_quotes(ptr);
    strip_space(ptr);

    printf("\"%s\"\n", ptr->buffer);

    append_string_fmt(ptr, " carp: %d and %s", 24, "another string");

    printf("\"%s\"\n", ptr->buffer);

    return 0;
}
#endif
