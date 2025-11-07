
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "alloc.h"
#include "array.h"

typedef struct _test_struct_t_ {
    char str[64];
    int len;
} ts_t;

static inline void error(const char* fmt, ...) {

    va_list args;

    fprintf(stderr, "array index error: ");

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
    exit(1);
}

static inline int normalize_read_index(array_t* ptr, int index) {

    if(index < 0)
        if(index == -1)
            return ptr->length - 1;
        else
            return ptr->length + index;
    else if(index >= 0 && index <= ptr->length)
        return index;
    else
        error("index out of range: %d", index);

    return 0; // never happens
}

static inline int normalize_write_index(array_t* ptr, int index) {

    if(index < 0)
        if(index == -1)
            return ptr->length;
        else
            return ptr->length + (index + 1);
    else if(index >= 0 && index <= ptr->length)
        return index;
    else
        error("index out of range: %d", index);

    return 0; // never happens
}

// requires a normalized index
static inline int byte_offset(array_t* ptr, int index) {

    return ptr->size * index;
}

// requires a normalized index
static inline void* make_ptr(array_t* ptr, int index) {

    return &ptr->buffer[byte_offset(ptr, index)];
}

static inline void resize(array_t* ptr, int size) {

    if(ptr->length + size > ptr->capacity) {
        while(ptr->length + size > ptr->capacity)
            ptr->capacity <<= 1;
        ptr->buffer = _REALLOC(ptr->buffer, ptr->size * ptr->capacity);
    }
}

// requires a normalized index
static inline int tail_length(array_t* ptr, int index) {

    return (ptr->length - index) * ptr->size;
}

/*************
 * Public interface functions
 */

/*
 * Allocate and initialize a new array data structure.
 */
array_t* create_array(size_t size, int (*comp)(void*, void*)) {

    array_t* ptr = _ALLOC_TYPE(array_t);
    ptr->capacity = 1 << 3;
    ptr->length = 0;
    ptr->size = size;
    ptr->comp = comp;
    ptr->is_sorted = 0;
    ptr->buffer = _ALLOC(ptr->capacity * ptr->size);

    return ptr;
}

/*
 * Free all memory associate with an array data structure.
 */
void destroy_array(array_t* arr) {

    if(arr != NULL) {
        _FREE(arr->buffer);
        _FREE(arr);
    }
}

/*
 * Insert an array into another array at the given index.
 */
void insert_array(array_t* dest, array_t* src, int index) {

    int idx = normalize_write_index(dest, index);

    resize(dest, src->length);
    memmove(make_ptr(dest, idx + src->length), make_ptr(dest, idx), tail_length(dest, idx));
    memcpy(make_ptr(dest, idx), make_ptr(src, 0), tail_length(src, 0));
    dest->length += src->length;
    dest->is_sorted = 0;
}

/*
 * Append an array at the end of another array.
 */
void append_array(array_t* dest, array_t* src) {

    resize(dest, src->length);
    memcpy(make_ptr(dest, dest->length), make_ptr(src, 0), tail_length(src, 0));
    dest->length += src->length;
    dest->is_sorted = 0;
}

/*
 * Make the array empty and reset the capacity to the default.
 */
void clear_array(array_t* arr) {

    arr->length = 0;
    arr->capacity = 1 << 3;
    arr->buffer = _REALLOC(arr->buffer, arr->size * arr->capacity);
    arr->is_sorted = 0;
}

/*
 * Sort the array according to the comp callback given when it was created.
 */
void sort_array(array_t* arr) {

    for(int step = 0; step < arr->length - 1; step++) {
        for(int i = 0; i < arr->length - step - 1; i++) {
            if(arr->comp(make_ptr(arr, i), make_ptr(arr, i + 1)) > 0) {
                swap_array_data(arr, i, i + 1);
            }
        }
    }
    arr->is_sorted = 1;
}

/*
 * Find the array element and return a pointer to it or NULL if not found.
 */
void* find_array(array_t* arr, void* key) {

    int idx = find_array_index(arr, key);

    if(idx >= 0)
        return index_array_data(arr, idx);
    else
        return NULL;
}

/*
 *  Find the array element and return the index or -1 if not found.
 */
int find_array_index(array_t* arr, void* key) {

    if(arr->comp == NULL)
        return -1;
    else if(!arr->is_sorted)
        sort_array(arr);

    int low = 0;
    int high = arr->length - 1;

    while(low <= high) {
        int mid = low + (high - low) / 2;
        int val = arr->comp(key, make_ptr(arr, mid));

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

/*
 * Append a data item to the end of the array
 */
void append_array_data(array_t* arr, void* data) {

    resize(arr, 1);
    memcpy(make_ptr(arr, arr->length), data, arr->size);
    arr->length++;
    arr->is_sorted = 0;
}

/*
 * Delete a single data item from the array at the given index.
 */
void delete_array_data(array_t* arr, int index) {

    int idx = normalize_write_index(arr, index);

    memcpy(make_ptr(arr, idx), make_ptr(arr, idx + 1), tail_length(arr, idx));
    arr->length--;
    arr->is_sorted = 0;
}

/*
 * Insert a data item into the array at the given index.
 */
void insert_array_data(array_t* arr, void* ptr, int index) {

    int idx = normalize_write_index(arr, index);

    resize(arr, 1);
    memmove(make_ptr(arr, idx + 1), make_ptr(arr, idx), tail_length(arr, idx));
    memcpy(make_ptr(arr, idx), ptr, arr->size);
    arr->length++;
    arr->is_sorted = 0;
}

/*
 * Return data items one-by-one, starting at the 0'th item and NULL when
 * they run out.
 */
void* iterate_array_data(array_t* arr, int* post) {

    if(*post < arr->length) {
        void* retv = index_array_data(arr, *post);
        *post = *post + 1;
        return retv;
    }
    else
        return NULL;
}

/*
 * Return the item at the given index or NULL if an invalid index is given.
 */
void* index_array_data(array_t* arr, int index) {

    void* ptr = NULL;

    if(len_array(arr) > 0)
        ptr = make_ptr(arr, normalize_read_index(arr, index));

    return ptr;
}

/*
 * Swap the data items at the given indexes. If either index is out of
 * range, then swap at the beginning or the end as approprate.
 */
void swap_array_data(array_t* arr, int idx1, int idx2) {

    int i1 = normalize_read_index(arr, idx1);
    int i2 = normalize_read_index(arr, idx2);

    void* tmp = _ALLOC(arr->size);
    memcpy(tmp, make_ptr(arr, i1), arr->size);
    memcpy(make_ptr(arr, i1), make_ptr(arr, i2), arr->size);
    memcpy(make_ptr(arr, i2), tmp, arr->size);
    _FREE(tmp);
    arr->is_sorted = 0;
}

/*
 * Append a data item to the end of the array.
 */
void push_array(array_t* arr, void* data) {

    append_array_data(arr, data);
}

/*
 * Remove the data item at the end of the array and return a pointer to
 * the new top-of-stack.
 */
void* pop_array(array_t* arr) {

    void* ptr = NULL;
    if(arr->length > 0) {
        ptr = &arr->buffer[arr->length-1];
        arr->length--;
    }

    return ptr;
}

/*
 * Return a pointer to the data item at the top of the stack.
 */
void* peek_array(array_t* arr) {

    void* ptr = NULL;
    if(arr->length > 0) {
        ptr = &arr->buffer[arr->length-1];
    }

    return ptr;
}

/*
 * Return a pointer to the start of the array.
 */
void* raw_array(array_t* arr) {

    return arr->buffer;
}

/*
 * Return how many data items there are in the array.
 */
int len_array(array_t* arr) {

    return arr->length;
}

/****************
 * Test section
 *
 * Build string:
 * clang -Wall -Wextra -Wpedantic -pedantic -g -o t array.c
 */
#if 0

#include <stdio.h>

// 25 unique words
char* strs[] = {
    "nonconcentrically",
    "prerogatival",
    "fillies",
    "aphrodite",
    "paragraphically",
    "shoddier",
    "chowchow",
    "applier",
    "despiritualize",
    "punchinello",
    "meninting",
    "erythrogonium",
    "perjury",
    "tenons",
    "nonundulate",
    "bistratose",
    "poecilonym",
    "coquelicot",
    "dubitative",
    "loasa",
    "noncalcareous",
    "brite",
    "borderism",
    "chordamesoderm",
    "cados",
    NULL
};

void dump_item(ts_t* ptr, int mark, const char* str) {

    if(str != NULL)
        printf("------ %s ------\n", str);
    printf("%3d. \"%s\": %d\n", mark, ptr->str, ptr->len);
    if(str != NULL)
        printf("\n");
}

void dump_array(array_t* arr, const char* str) {

    int mark = 0;
    ts_t* ptr;

    printf("------ %s ------\n", str);
    printf("cap: %d len: %d size: %d sorted: %d\n", arr->capacity, arr->length, arr->size, arr->is_sorted);
    while(NULL != (ptr = iterate_array_data(arr, &mark))) {
        dump_item(ptr, mark-1, NULL);
    }
    printf("\n");
}

int comp(void*p1, void* p2) {
    return strcmp(((ts_t*)p1)->str, ((ts_t*)p2)->str);
}

int main(void) {

    ts_t tmp;
    array_t* arr = create_array(sizeof(ts_t), comp);
    void* ptr;

    for(int i = 0; i < 10; i++) {
        strncpy(tmp.str, strs[i], sizeof(tmp.str));
        tmp.len = strlen(strs[i]);
        append_array_data(arr, &tmp);
    }
    dump_array(arr, "new array");

    for(int i = 0; i < 3; i++) {
        strncpy(tmp.str, "insert at 5", sizeof(tmp.str));
        tmp.len = strlen(tmp.str);
        insert_array_data(arr, &tmp, 5);
    }
    dump_array(arr, "insert 3 at 5");

    ptr = index_array_data(arr, 0);
    dump_item(ptr, 0, "item at 0");
    ptr = index_array_data(arr, 4);
    dump_item(ptr, 4, "item at 4");
    ptr = index_array_data(arr, -1);
    dump_item(ptr, -1, "item at -1");
    ptr = index_array_data(arr, -3);
    dump_item(ptr, -3, "item at -3");

    strncpy(tmp.str, "insert 3 at 0", sizeof(tmp.str));
    tmp.len = strlen(tmp.str);
    insert_array_data(arr, &tmp, 0);
    insert_array_data(arr, &tmp, 0);
    insert_array_data(arr, &tmp, 0);
    dump_array(arr, "insert 3 at 0");

    strncpy(tmp.str, "insert 3 at -1", sizeof(tmp.str));
    tmp.len = strlen(tmp.str);
    insert_array_data(arr, &tmp, -1);
    insert_array_data(arr, &tmp, -1);
    insert_array_data(arr, &tmp, -1);
    dump_array(arr, "insert at 3 at -1");

    delete_array_data(arr, 6);
    delete_array_data(arr, 6);
    delete_array_data(arr, 6);
    dump_array(arr, "delete 3 at 6");

    delete_array_data(arr, 0);
    delete_array_data(arr, 0);
    delete_array_data(arr, 0);
    delete_array_data(arr, -1);
    delete_array_data(arr, -1);
    delete_array_data(arr, -1);
    delete_array_data(arr, 3);
    delete_array_data(arr, 3);
    dump_array(arr, "delete 3 at 0 and -1 and 2 at 3");

    swap_array_data(arr, 2, 5);
    dump_array(arr, "swap 2 and 5");
    swap_array_data(arr, 0, 2);
    dump_array(arr, "swap 0 and 2");
    swap_array_data(arr, 7, 5);
    dump_array(arr, "swap 7 and 5");
    swap_array_data(arr, -1, 3);
    dump_array(arr, "swap -1 and 3");

    swap_array_data(arr, 3, -1);
    dump_array(arr, "swap 3 and -1");

    array_t* ptr1 = create_array((sizeof(ts_t)), comp);
    for(int i = 15; i < 20; i++) {
        strncpy(tmp.str, strs[i], sizeof(tmp.str));
        tmp.len = strlen(strs[i]);
        append_array_data(ptr1, &tmp);
    }
    dump_array(ptr1, "second array");

    append_array(arr, ptr1);
    dump_array(arr, "append array");

    for(int i = 0; i < 5; i++)
        delete_array_data(arr, 8);
    dump_array(arr, "delete 5 at 8");

    for(int i = 0; i < 5; i++)
        delete_array_data(arr, 0);
    dump_array(arr, "delete 5 at 0");

    insert_array(arr, ptr1, 3);
    dump_array(arr, "insert array at 3");

    for(int i = 0; i < 5; i++)
        delete_array_data(arr, 3);
    dump_array(arr, "delete 5 at 3");

    insert_array(arr, ptr1, -1);
    dump_array(arr, "insert array at -1");

    for(int i = 0; i < 5; i++)
        delete_array_data(arr, -1);
    dump_array(arr, "delete 5 at -1");

    insert_array(arr, ptr1, -4);
    dump_array(arr, "insert array at -4");

    for(int i = 0; i < 5; i++)
        delete_array_data(arr, -4);
    dump_array(arr, "delete 5 at -4");

    clear_array(arr);
    dump_array(arr, "cleared array");

    for(int i = 0; i < 10; i++) {
        strncpy(tmp.str, strs[i], sizeof(tmp.str));
        tmp.len = strlen(strs[i]);
        append_array_data(arr, &tmp);
    }
    dump_array(arr, "new array");

    append_array(arr, ptr1);
    dump_array(arr, "append array");

    sort_array(arr);
    dump_array(arr, "sort array");

    strcpy(tmp.str, "fillies");
    ptr = find_array(arr, &tmp);
    dump_item(ptr, 0, "find \"fillies\"");

    strcpy(tmp.str, "dubitative");
    ptr = find_array(arr, &tmp);
    dump_item(ptr, 0, "find \"dubitative\"");

    strcpy(tmp.str, "aphrodite");
    ptr = find_array(arr, &tmp);
    dump_item(ptr, 0, "find \"aphrodite\"");

    strcpy(tmp.str, "shoddier");
    ptr = find_array(arr, &tmp);
    dump_item(ptr, 0, "find \"shoddier\"");

    strcpy(tmp.str, "push one on stack");
    tmp.len = strlen(tmp.str);
    push_array(arr, &tmp);

    strcpy(tmp.str, "push two on stack");
    tmp.len = strlen(tmp.str);
    push_array(arr, &tmp);

    strcpy(tmp.str, "push three on stack");
    tmp.len = strlen(tmp.str);
    push_array(arr, &tmp);
    dump_array(arr, "push three");

    ptr = pop_array(arr);
    dump_item(ptr, 0, "pop stack");

    ptr = pop_array(arr);
    dump_item(ptr, 0, "pop stack");

    ptr = peek_array(arr);
    dump_item(ptr, 0, "peek stack");

    ptr = pop_array(arr);
    dump_item(ptr, 0, "pop stack");

    dump_array(arr, "pop three");

    destroy_array(arr);
    destroy_array(ptr1);


    return 0;
}

#endif
