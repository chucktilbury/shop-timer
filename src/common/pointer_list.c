
#include "alloc.h"
#include "pointer_list.h"

// The index could be <0, which refers to the end of the list.
static int normalize_index(pointer_list_t* lst, int idx) {

    if(idx >= 0 && idx < lst->len)
        return idx;
    else if(idx < 0 && -idx < lst->len) {
        idx = lst->len - idx;
        return idx;
    }
    else
        return -1;
}

pointer_list_t* create_ptr_list(void) {

    pointer_list_t* ptr = _ALLOC_TYPE(pointer_list_t);
    ptr->len = 0;
    ptr->cap = 1 << 3;
    ptr->buffer = _ALLOC_ARRAY(void*, ptr->cap);
    ptr->is_sorted = false;

    return ptr;
}

void destroy_ptr_list(pointer_list_t* lst) {

    if(lst != NULL) {
        _FREE(lst->buffer);
        _FREE(lst);
    }
}

void append_ptr_list(pointer_list_t* lst, void* ptr) {

    if(lst->len + 1 > lst->cap) {
        lst->cap <<= 1;
        lst->buffer = _REALLOC_ARRAY(lst->buffer, void*, lst->cap);
    }

    lst->buffer[lst->len] = ptr;
    lst->len++;
    lst->is_sorted = false;
}

void* index_ptr_list(pointer_list_t* lst, int index) {

    int idx = normalize_index(lst, index);

    if(idx >= 0)
        return lst->buffer[idx];
    else
        return NULL;
}

void push_ptr_list(pointer_list_t* lst, void* ptr) {

    append_ptr_list(lst, ptr);
    lst->is_sorted = false;
}

void* pop_ptr_list(pointer_list_t* lst) {

    void *ptr = NULL;

    if(lst->len > 0) {
        ptr = lst->buffer[lst->len - 1];
        lst->len--;
    }

    return ptr;
}

void* peek_ptr_list(pointer_list_t* lst) {

    void *ptr = NULL;

    if(lst->len > 0)
        ptr = lst->buffer[lst->len - 1];

    return ptr;
}

void* iterate_ptr_list(pointer_list_t* lst, int* post) {

    if(lst == NULL)
        return NULL;

    void* ptr = NULL;

    if((*post >= 0) && (*post < lst->len)) {
        ptr = lst->buffer[*post];
        *post = *post + 1;
    }

    return ptr;
}

int len_ptr_list(pointer_list_t* lst) {

    return (int)lst->len;
}

// bubble sort
void sort_ptr_list(pointer_list_t* lst, int (*comp_func)(void*, void*)) {

    for(int step = 0; step < lst->len - 1; step++) {
        for(int i = 0; i < lst->len - step - 1; i++) {
            if(comp_func(lst->buffer[i], lst->buffer[i + 1]) > 0) {
                void* tmp = lst->buffer[i];
                lst->buffer[i] = lst->buffer[i + 1];
                lst->buffer[i + 1] = tmp;
            }
        }
    }
    lst->is_sorted = true;
}

// binary search
int find_ptr_list_idx(pointer_list_t* lst, void* key, int (*comp_func)(void*, void*)) {

    if(!lst->is_sorted)
        return -1;

    int low = 0;
    int high = lst->len - 1;

    while(low <= high) {
        int mid = low + (high - low) / 2;
        int val = comp_func(key, lst->buffer[mid]);

        if(val == 0)
            return mid;

        if(val > 0)
            low = mid + 1;
        else
            high = mid - 1;
    }

    // not found
    return -1;
}

void* find_ptr_list(pointer_list_t* lst, void* key, int (*comp_func)(void*, void*)) {

    int idx = find_ptr_list_idx(lst, key, comp_func);

    if(idx >= 0)
        return index_ptr_list(lst, idx);
    else
        return NULL;
}

/*
 * Note that this does not copy the pointers in the list, only the list
 * itself.
 */
pointer_list_t* copy_ptr_list(pointer_list_t* lst) {

    pointer_list_t* ptr = create_ptr_list();

    for(int i = 0; i < lst->len; i++)
        append_ptr_list(ptr, lst->buffer[i]);

    return ptr;
}
