
#ifndef _STRING_BUFFER_H_
#define _STRING_BUFFER_H_

#include <stdio.h>
#include <stddef.h>

typedef struct {
    char* buffer;
    int len;
    int cap;
} string_t;

string_t* create_string(const char* str);
string_t* create_string_fmt(const char* fmt, ...);
void destroy_string(string_t* buf);
string_t* append_string(string_t* buf, const char* str);
string_t* append_string_str(string_t* buf, string_t* str);
string_t* append_string_fmt(string_t* buf, const char* fmt, ...);
string_t* append_string_char(string_t* buf, int ch);
void clear_string(string_t* buf);
int len_string(string_t* buf);
int comp_string(string_t* buf1, string_t* buf2);
int comp_string_str(string_t* buf1, const char* buf2);
const char* raw_string(string_t* str);
void emit_string(FILE* fp, string_t* ptr);
void emit_string_fmt(FILE* fp, const char* fmt, ...);

string_t* strip_char(string_t* buf, int ch);
string_t* upcase(string_t* buf);
string_t* downcase(string_t* buf);
string_t* strip_space(string_t* buf);
string_t* convert(string_t* str);
string_t* copy_string(string_t* buf);

#endif /* _STRING_BUFFER_H_ */
