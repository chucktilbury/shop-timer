#ifndef _POINTER_LIST_H_
#define _POINTER_LIST_H_

#include <stddef.h>
#include <stdbool.h>

typedef struct _ptr_list_t_ {
    void** buffer;
    int len;
    int cap;
    bool is_sorted;
} pointer_list_t;

pointer_list_t* create_ptr_list(void);
void destroy_ptr_list(pointer_list_t* lst);
void append_ptr_list(pointer_list_t* lst, void* ptr);
void* index_ptr_list(pointer_list_t* lst, int index);
void push_ptr_list(pointer_list_t* lst, void* ptr);
void* pop_ptr_list(pointer_list_t* lst);
void* peek_ptr_list(pointer_list_t* lst);
void* iterate_ptr_list(pointer_list_t* lst, int* post);
int len_ptr_list(pointer_list_t* lst);
void sort_ptr_list(pointer_list_t* lst, int (*comp_func)(void*, void*));
void* find_ptr_list(pointer_list_t* lst, void* key, int (*comp_func)(void*, void*));
int find_ptr_list_idx(pointer_list_t* lst, void* key, int (*comp_func)(void*, void*));
pointer_list_t* copy_ptr_list(pointer_list_t* lst);

#endif /* _POINTER_LIST_H_ */
