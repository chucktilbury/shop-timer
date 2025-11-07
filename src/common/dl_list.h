
#ifndef _DL_LIST_H_
#define _DL_LIST_H_

#include <stddef.h>

typedef struct _dl_list_node_t_ {
    void* data;
    size_t size;
    struct _dl_list_node_t_* next;
    struct _dl_list_node_t_* prev;
} dl_list_node_t;

typedef struct {
    struct _dl_list_node_t_* first;
    struct _dl_list_node_t_* last;
    struct _dl_list_node_t_* crnt;
    int num_elem;
} dl_list_t;

dl_list_t* create_dl_list(void);
void destroy_dl_list(dl_list_t* lst);

dl_list_node_t* create_dl_list_node(void* data, size_t size);
void destroy_dl_list_node(dl_list_node_t* node);
void* delete_crnt_dl_list_node(dl_list_t* lst);

// operate at current node
void* insert_dl_list(dl_list_t* lst, dl_list_node_t* node);
void append_dl_list(dl_list_t* lst, dl_list_node_t* node);
void prepend_dl_list(dl_list_t* lst, dl_list_node_t* node);
void* iterate_dl_list(dl_list_t* lst, void** mark);

void* reset_dl_list(dl_list_t* lst, int flag);
void* crnt_dl_list_node(dl_list_t* lst);
void* prev_dl_list_node(dl_list_t* lst);
void* next_dl_list_node(dl_list_t* lst);

#endif /* _DL_LIST_H_ */
