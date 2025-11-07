/**
 * @file strlist.h
 *
 * @brief Thin wrapper for pointer lists.
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.1
 * @date 2025-04-01
 * @copyright Copyright (c) 2025
 */
#ifndef _STRG_LIST_H_
#define _STRG_LIST_H_

#include "strgs.h"
#include "ptr_list.h"

typedef ptr_list_t str_list_t;

str_list_t* create_str_list(void);
void append_str_list(str_list_t* lst, string_t* str);
string_t* index_str_list(str_list_t* lst, int index);
string_t* iterate_str_list(str_list_t* lst, int* post);
int len_str_list(str_list_t* lst);
void sort_str_list(str_list_t* lst);
int find_str_list(str_list_t* lst, string_t* key);

void add_str_list(str_list_t* lst, string_t* str);

#endif /* _STRG_LIST_H_ */
