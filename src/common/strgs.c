/**
 * @file strbuf.c
 *
 * @brief
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.1
 * @date 2025-03-25
 * @copyright Copyright (c) 2025
 */
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#include "alloc.h"
#include "strgs.h"

string_t* create_string(const char* str) {

    string_t* ptr = _ALLOC_TYPE(string_t);
    ptr->cap      = 1 << 3;
    ptr->len      = 0;
    ptr->buffer   = _ALLOC_ARRAY(char, ptr->cap);

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

    return buf;
}

void clear_string(string_t* buf) {

    buf->len       = 0;
    buf->buffer[0] = 0;
}

int len_string(string_t* buf) {

    return buf->len;
}

int comp_string(string_t* buf1, string_t* buf2) {

    return strcmp(buf1->buffer, buf2->buffer);
}

const char* raw_string(string_t* str) {

    return str->buffer;
}

string_t* strip_quotes(string_t* buf) {

    strip_space(buf);

    char* temp = strrchr(buf->buffer, '\"');
    if(temp != NULL)
        *temp = '\0';
    else if(NULL != (temp = strrchr(buf->buffer, '\'')))
        *temp = '\0';

    if(buf->buffer[0] == '\'' || buf->buffer[0] == '\"')
        memmove(&buf->buffer[0], &buf->buffer[1], strlen(buf->buffer));

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

string_t* convert(string_t* str) {

    string_t* buf = create_string(NULL);

    for(int i = 0; str->buffer[i] != '\0'; i++) {
        switch(str->buffer[i]) {
            case '~':
                append_string(buf, "TILDE");
                break;
            case '`':
                append_string(buf, "BQUOTE");
                break;
            case '!':
                append_string(buf, "BANG");
                break;
            case '@':
                append_string(buf, "AT");
                break;
            case '#':
                append_string(buf, "POUND");
                break;
            case '$':
                append_string(buf, "DOLLAR");
                break;
            case '%':
                append_string(buf, "PERCENT");
                break;
            case '^':
                append_string(buf, "CARET");
                break;
            case '&':
                append_string(buf, "AMP");
                break;
            case '*':
                append_string(buf, "STAR");
                break;
            case '(':
                append_string(buf, "OPAREN");
                break;
            case ')':
                append_string(buf, "CPAREN");
                break;
            case '-':
                append_string(buf, "MINUS");
                break;
            case '=':
                append_string(buf, "EQUAL");
                break;
            case '+':
                append_string(buf, "PLUS");
                break;
            case '[':
                append_string(buf, "OSBRACE");
                break;
            case '{':
                append_string(buf, "OCBRACE");
                break;
            case ']':
                append_string(buf, "CSBRACE");
                break;
            case '}':
                append_string(buf, "CCBRACE");
                break;
            case '\\':
                append_string(buf, "BSLASH");
                break;
            case '|':
                append_string(buf, "BAR");
                break;
            case ';':
                append_string(buf, "SCOLON");
                break;
            case ':':
                append_string(buf, "COLON");
                break;
            case '\'':
                append_string(buf, "SQUOTE");
                break;
            case '\"':
                append_string(buf, "DQUOTE");
                break;
            case ',':
                append_string(buf, "COMMA");
                break;
            case '<':
                append_string(buf, "OPBRACE");
                break;
            case '.':
                append_string(buf, "DOT");
                break;
            case '>':
                append_string(buf, "CPBRACE");
                break;
            case '/':
                append_string(buf, "SLASH");
                break;
            case '?':
                append_string(buf, "QUESTION");
                break;
        }
        if(str->buffer[i + 1] != '\0')
            append_string_char(buf, '_');
    }

    return buf;
}

string_t* copy_string(string_t* buf) {

    return create_string(buf->buffer);
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
