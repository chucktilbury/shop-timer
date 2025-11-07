
#ifndef _STRING_LIST_H_
#define _STRING_LIST_H_

#include "string_buffer.h"
#include "pointer_list.h"

typedef pointer_list_t string_list_t;

string_list_t* create_string_list(void);
void destroy_string_list(string_list_t* lst);
void append_string_list(string_list_t* lst, string_t* str);
string_t* index_string_list(string_list_t* lst, int index);
string_t* iterate_string_list(string_list_t* lst, int* post);
int len_string_list(string_list_t* lst);
void sort_string_list(string_list_t* lst);
int find_string_list(string_list_t* lst, string_t* key);

void add_string_list(string_list_t* lst, string_t* str);
string_list_t* split_string(const char* str, int ch);
void append_string_list_list(string_list_t* dest, string_list_t* src);

#endif /* _STRING_LIST_H_ */
