
#include <string.h>

#include "string_list.h"
#include "pointer_list.h"
#include "string_buffer.h"

string_list_t* create_string_list(void) {

    return (string_list_t*)create_ptr_list();
}

void destroy_string_list(string_list_t* lst) {

    if(lst != NULL) {
        int mark = 0;
        string_t* item;

        while(NULL != (item = iterate_string_list(lst, &mark)))
            destroy_string(item);
        destroy_ptr_list(lst);
    }
}

void append_string_list(string_list_t* lst, string_t* str) {

    append_ptr_list((pointer_list_t*)lst, (void*)str);
}

string_t* index_string_list(string_list_t* lst, int index) {

    return (string_t*)index_ptr_list((pointer_list_t*)lst, index);
}

string_t* iterate_string_list(string_list_t* lst, int* post) {

    return (string_t*)iterate_ptr_list((pointer_list_t*)lst, post);
}

int len_string_list(string_list_t* lst) {

    return len_ptr_list((pointer_list_t*)lst);
}

static int comp_func(void* p1, void* p2) {

    return comp_string((string_t*)p1, (string_t*)p2);
}

void sort_string_list(string_list_t* lst) {

    sort_ptr_list((pointer_list_t*)lst, comp_func);
}

int find_string_list(string_list_t* lst, string_t* key) {

    return find_ptr_list_idx((pointer_list_t*)lst, (void*)key, comp_func);
}


/*
 * Add to the string into the list if it does not already exist.
 */
void add_string_list(string_list_t* lst, string_t* str) {

    int mark = 0;
    string_t* ptr;

    while(NULL != (ptr = iterate_string_list(lst, &mark)))
        if(comp_string(ptr, str) == 0)
            return;

    append_string_list(lst, str);
}

string_list_t* split_string(const char* str, int ch) {

    string_list_t* sl = create_string_list();
    string_t* buf = create_string(str);
    char* head = (char*)raw_string(buf);
    char* tail = head;
    int finished = 0;

    while(!finished) {
        tail = strchr(head, ch);
        if(tail != NULL) {
            *tail = '\0';
            append_string_list(sl, create_string(head));
            tail++;
            head = tail;
            if(*head == '\0')
                finished++;
        }
        else {
            append_string_list(sl, create_string(head));
            finished++;
        }
    }

    destroy_string(buf);
    return sl;
}

// append the src string list to the dest string list
// does not change src.
void append_string_list_list(string_list_t* dest, string_list_t* src) {

    string_t* item;
    int mark = 0;

    while(NULL != (item = iterate_string_list(src, &mark)))
        append_string_list(dest, copy_string(item));
}
