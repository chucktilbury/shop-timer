
#include "dl_list.h"
#include "alloc.h"

dl_list_t* create_dl_list(void) {

    dl_list_t* ptr = _ALLOC_TYPE(dl_list_t);

    return ptr;
}

void destroy_dl_list(dl_list_t* lst) {

    dl_list_node_t* ptr;
    dl_list_node_t* next;

    for(ptr = lst->first; ptr != NULL; ptr = next) {
        next = ptr->next;
        destroy_dl_list_node(ptr);
    }

    _FREE(lst);
}


dl_list_node_t* create_dl_list_node(void* data, size_t size) {

    dl_list_node_t* ptr = _ALLOC_TYPE(dl_list_node_t);
    ptr->data = _COPY(data, size);
    ptr->size = size;

    return ptr;
}

void destroy_dl_list_node(dl_list_node_t* node) {

    _FREE(node->data);
    _FREE(node);
}


void* delete_crnt_dl_list_node(dl_list_t* lst) {

    if(lst->first == NULL)
        return NULL;

    if(lst->crnt != NULL) {
        dl_list_node_t* crnt = lst->crnt;
        dl_list_node_t* next = lst->crnt->next;
        dl_list_node_t* prev = lst->crnt->prev;
        lst->crnt = next != NULL ? next : prev;

        if(next == NULL) {
            lst->last = prev;
            lst->last->next = NULL;
        }
        else if(prev == NULL) {
            lst->first = next;
            lst->first->prev = NULL;
        }
        else {
            next->prev = prev;
            prev->next = next;
        }

        destroy_dl_list_node(crnt);
    }

    if(lst->crnt != NULL)
        return lst->crnt->data;
    else
        return NULL;
}

// insert after the current node
void* insert_dl_list_node(dl_list_t* lst, dl_list_node_t* node) {

    if(lst->crnt != NULL) {
        dl_list_node_t* crnt = lst->crnt;
        dl_list_node_t* next = lst->crnt->next;
        dl_list_node_t* prev = lst->crnt->prev;

        if(prev == NULL)
            prepend_dl_list(lst, node);
        else if(next == NULL)
            append_dl_list(lst, node);
        else {
            node->prev = crnt;
            node->next = next;
            crnt->next = node;
            next->prev = node;
        }
        return node->data;
    }
    else
        return NULL;
}

void append_dl_list(dl_list_t* lst, dl_list_node_t* node) {

    node->prev = lst->last;
    if(lst->last != NULL)
        lst->last->next = node;
    else {
        lst->crnt = node;
        lst->first = node;
    }
    lst->last = node;
}

void prepend_dl_list(dl_list_t* lst, dl_list_node_t* node) {

    node->next = lst->first;
    if(lst->first != NULL)
        lst->first->prev = node;
    else {
        lst->crnt = node;
        lst->last = node;
    }
    lst->first = node;
}

void* iterate_dl_list(dl_list_t* lst, void** mark) {

    dl_list_node_t* node = ((dl_list_node_t*)*mark);
    if(node != (void*)-1) {
        if(node == NULL) {
            *mark = (void*)lst->first;
            return ((dl_list_node_t*)*mark)->data;
        }
        else if(node->next != NULL) {
            *mark = (void*)node->next;
            return ((dl_list_node_t*)*mark)->data;
        }
        else {
            *mark = (void*)-1;
            return NULL;
        }
    }

    return NULL;
}

void* reset_dl_list(dl_list_t* lst, int flag) {

    if(flag)
        lst->crnt = lst->last;
    else
        lst->crnt = lst->first;

    return lst->crnt->data;
}

void* crnt_dl_list_node(dl_list_t* lst) {

    return lst->crnt->data;
}

void* prev_dl_list_node(dl_list_t* lst) {

    if(lst->crnt != NULL)
        lst->crnt = lst->crnt->prev;
    return lst->crnt->data;
}

void* next_dl_list_node(dl_list_t* lst) {

    if(lst->crnt != NULL)
        lst->crnt = lst->crnt->next;
    return lst->crnt->data;
}


#if 0
// build string:
// clang -g -Wall -Wextra -Wpedantic -pedantic -o t dl_list.c alloc.c

#include <stdio.h>
#include <string.h>

typedef struct {
    char str[64];
    int size;
} tst_t;

char* strlist[] = {
    "baca",
    "abacay",
    "abacas",
    "abacate",
    "abacaxi",
    "abaci",
    "abacinate",
    "abacination",
    "abacisci",
    "abaciscus",
    "abacist",
    "aback",
    "abacli",
    "abacot",
    "abacterial",
    "abactinal",
    "abactinally",
    "abaction",
    "abactor",
    "abaculi",
    "abaculus",
    "abacus",
    "abacuses",
    "abada",
    "abaddon",
    "abadejo",
    "abadengo",
    "abadia",
    "abadite",
    "abaff",
    "abaft",
    "abay",
    "abayah",
    NULL
};

tst_t* create_ts(const char* str) {

    tst_t* ptr = _ALLOC_TYPE(tst_t);
    int len = strlen(str);
    int room = sizeof(ptr->str) - 1;
    int size = (len > room)? room: len;
    memcpy(ptr->str, str, size);
    ptr->size = size;

    return ptr;
}

void show_list(dl_list_t* lst) {

    reset_dl_list(lst, 0);
    tst_t* node;
    void* mark = NULL;
    int count = 1;

    while(NULL != (node = iterate_dl_list(lst, &mark)))
        printf("%3d. %s: %d\n", count++, node->str, node->size);
}

int main(void) {

    dl_list_t* lst = create_dl_list();

    for(int i = 0; i < 7; i++)
        append_dl_list(lst, create_dl_list_node(create_ts(strlist[i]), sizeof(tst_t)));

    show_list(lst);

    for(int i = 7; i < 16; i++)
        prepend_dl_list(lst, create_dl_list_node(create_ts(strlist[i]), sizeof(tst_t)));

    printf("------\n");

    show_list(lst);

    printf("------\n");

    reset_dl_list(lst, 0);
    tst_t* node = crnt_dl_list_node(lst);
    printf("%s: %d\n", node->str, node->size);

    for(int i = 0; i < 5; i++)
        next_dl_list_node(lst);
    node = crnt_dl_list_node(lst);
    printf("%s: %d\n", node->str, node->size);

    printf("------\n");

    reset_dl_list(lst, 1);
    node = crnt_dl_list_node(lst);
    printf("%s: %d\n", node->str, node->size);

    for(int i = 0; i < 2; i++)
        prev_dl_list_node(lst);
    node = crnt_dl_list_node(lst);
    printf("%s: %d\n", node->str, node->size);

    printf("------ insert tests\n");

    reset_dl_list(lst, 0);
    for(int i = 0; i < 5; i++)
        next_dl_list_node(lst);
    node = crnt_dl_list_node(lst);
    printf("%s: %d\n", node->str, node->size);
    insert_dl_list_node(lst, create_dl_list_node(create_ts("insert index 5"), sizeof(tst_t)));
    show_list(lst);

    printf("------\n");
    reset_dl_list(lst, 0);
    node = crnt_dl_list_node(lst);
    printf("%s: %d\n", node->str, node->size);
    insert_dl_list_node(lst, create_dl_list_node(create_ts("insert head"), sizeof(tst_t)));
    show_list(lst);

    printf("------\n");
    reset_dl_list(lst, 1);
    node = crnt_dl_list_node(lst);
    printf("%s: %d\n", node->str, node->size);
    insert_dl_list_node(lst, create_dl_list_node(create_ts("insert tail"), sizeof(tst_t)));
    show_list(lst);

    printf("------ delete tests\n");

    reset_dl_list(lst, 0);
    for(int i = 0; i < 7; i++)
        next_dl_list_node(lst);
    node = crnt_dl_list_node(lst);
    printf("%s: %d\n", node->str, node->size);
    delete_crnt_dl_list_node(lst);
    show_list(lst);

    printf("------\n");
    reset_dl_list(lst, 0);
    node = crnt_dl_list_node(lst);
    printf("%s: %d\n", node->str, node->size);
    delete_crnt_dl_list_node(lst);
    show_list(lst);

    printf("------\n");
    reset_dl_list(lst, 1);
    node = crnt_dl_list_node(lst);
    printf("%s: %d\n", node->str, node->size);
    delete_crnt_dl_list_node(lst);
    show_list(lst);

    destroy_dl_list(lst);

    return 0;
}

#endif
