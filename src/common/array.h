
#ifndef _ARRAY_H_
#define _ARRAY_H_

#include <stddef.h>

typedef struct _array_t_ {
    unsigned char* buffer; // array of bytes
    int capacity;          // capacity of array in items
    int length;            // length of the array in items
    int size;              // size of each item
    int (*comp)(void*,void*); // compare function.
    int is_sorted; // flag indicates if a sorted search is okay.
} array_t;

array_t* create_array(size_t size, int(*comp)(void*,void*));
void destroy_array(array_t* arr);
void insert_array(array_t* arr, array_t* ptr, int index);
void append_array(array_t* dest, array_t* src);
void clear_array(array_t* arr);
void sort_array(array_t* arr);
void* find_array(array_t* arr, void* key);
int find_array_index(array_t* arr, void* key);

void append_array_data(array_t* arr, void* data);
void delete_array_data(array_t* arr, int index);
void insert_array_data(array_t* arr, void* ptr, int index);
void* iterate_array_data(array_t* arr, int* post);
void* index_array_data(array_t* h, int index);
void swap_array_data(array_t* ptr, int idx1, int idx2);

void push_array(array_t* arr, void* data);
void* pop_array(array_t* arr);
void* peek_array(array_t* arr);

void* raw_array(array_t* arr);
int len_array(array_t* arr);


#endif /* _ARRAY_H_ */
