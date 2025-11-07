/**
 * @file strlist.c
 *
 * @brief Thin wrapper for pointer lists.
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.1
 * @date 2025-04-01
 * @copyright Copyright (c) 2025
 */
#include <string.h>

#include "strg_list.h"

str_list_t* create_str_list(void) {

    return (str_list_t*)create_ptr_list();
}

void append_str_list(str_list_t* lst, string_t* str) {

    append_ptr_list((ptr_list_t*)lst, (void*)str);
}

string_t* index_str_list(str_list_t* lst, int index) {

    return (string_t*)index_ptr_list((ptr_list_t*)lst, index);
}

string_t* iterate_str_list(str_list_t* lst, int* post) {

    return (string_t*)iterate_ptr_list((ptr_list_t*)lst, post);
}

int len_str_list(str_list_t* lst) {

    return len_ptr_list((ptr_list_t*)lst);
}

static int comp_func(void* p1, void* p2) {

    return comp_string((string_t*)p1, (string_t*)p2);
}

void sort_str_list(str_list_t* lst) {

    sort_ptr_list((ptr_list_t*)lst, comp_func);
}

int find_str_list(str_list_t* lst, string_t* key) {

    return find_ptr_list((ptr_list_t*)lst, (void*)key, comp_func);
}


/*
 * Add to the string into the list if it does not already exist.
 */
void add_str_list(str_list_t* lst, string_t* str) {

    int mark = 0;
    string_t* ptr;

    while(NULL != (ptr = iterate_str_list(lst, &mark)))
        if(comp_string(ptr, str) == 0)
            return;

    append_str_list(lst, str);
}
