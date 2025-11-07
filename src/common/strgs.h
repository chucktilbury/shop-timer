#ifndef _STRGS_H_
#define _STRGS_H_

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
string_t* append_string_fmt(string_t* buf, const char* fmt, ...);
string_t* append_string_char(string_t* buf, int ch);
void clear_string(string_t* buf);
int len_string(string_t* buf);
int comp_string(string_t* buf1, string_t* buf2);
const char* raw_string(string_t* str);

string_t* strip_quotes(string_t* buf);
string_t* upcase(string_t* buf);
string_t* strip_space(string_t* buf);
string_t* convert(string_t* str);
string_t* copy_string(string_t* buf);

#endif /* _STRGS_H_ */
