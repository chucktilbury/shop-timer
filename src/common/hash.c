/*
 *
 *  https://programming.guide/hash-tables-open-addressing.html
 *
 * The hash table uses linear probing where the probing distance is
 * hash & 0x0F. If the result is 0 then the distance is 1. When a hash
 * is deleted, the memory is freed and the tombstone flag is set. When
 * a hash is added, it can be added to a bucket which is a tombstone.
 *
 * The table is full when 3/4 of the buckets are in use. The table is
 * resized and all of the existing hashes are rehashed into the new table.
 *
 * The add function tracks the max number of hops that are needed to
 * insert a new hash. If the hops exceed a certain number, then the hash
 * table should be rehashed, but only if a certain number of adds have
 * taken place to avoid performance problems.
 *
 * Maybe tombstones should be counted instead of hops, but I do not
 * anticipate needing to delete a lot of entries. Rehashing deletes
 * tombstones.
 *
 * Test build string:
 * clang -Wall -Wextra -g -o t hash.c memory.c errors.c
 */

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"
#include "alloc.h"

// #define TEST_HASH

static uint32_t hash_func(const char* key) {

    uint32_t hash = 2166136261u;
    int slen = strlen(key);

    for(int i = 0; i < slen; i++) {
        hash ^= (uint8_t)key[i];
        hash *= 16777619;
    }

    return hash;
}

static int find_slot(hash_table_t* tab, const char* key) {

    uint32_t hash = hash_func(key) & (tab->cap - 1);
    int inc = hash & 0x0F;
    inc = (inc == 0) ? 1 : inc;

    if(tab->table[hash] == NULL) {
        return hash;
    }
    else {
        do {
            for(int i = 0; i < tab->cap; i++) {
                if(tab->table[hash] == NULL) {
                    return hash;
                }
                else if(tab->table[hash]->key == NULL) {
                    tab->tombstones--;
                    return hash;
                }
                else if(strcmp(tab->table[hash]->key, key) == 0) {
                    return hash; // duplicate key
                }
                else
                    hash = (hash + inc) & (tab->cap - 1);
            }
            inc = 1; // slot not found
        } while(true);
    }

    return -1; // keep the compiler happy
}

static void rehash_table(hash_table_t* tab) {

    if(tab->count * 1.75 >= tab->cap) {
        int oldcap = tab->cap;
        _hash_node_t** oldtab = tab->table;
        tab->cap <<= 1; // double the capacity
        tab->tombstones = 0;
        tab->count = 0;
        tab->table = _ALLOC_ARRAY(_hash_node_t*, tab->cap);

        int slot;

        for(int i = 0; i < oldcap; i++) {
            if(oldtab[i] != NULL && oldtab[i]->key != NULL) {
                slot = find_slot(tab, oldtab[i]->key);
                tab->table[slot] = oldtab[i];
                tab->count++;
            }
        }
        _FREE(oldtab);
    }
}

hash_table_t* create_hashtable(void) {

    hash_table_t* tab = _ALLOC_TYPE(hash_table_t);

    tab->count = 0;
    tab->cap = 0x01 << 2;

    tab->table = _ALLOC_ARRAY(_hash_node_t*, tab->cap);

    return tab;
}

void destroy_hashtable(hash_table_t* table) {

    if(table != NULL) {
        for(int i = 0; i < table->cap; i++) {
            if(table->table[i] != NULL) {
                if(table->table[i]->key != NULL) {
                    _FREE(table->table[i]->key);
                }
            }
            _FREE(table->table[i]);
        }

        _FREE(table->table);
        _FREE(table);
    }
}

int insert_hashtable(hash_table_t* table, const char* key, void* data) {

    rehash_table(table);

    int slot = find_slot(table, key);
    if(slot < 0) {
        // printf("cannot find slot for key: \"%s\"\n", key);
        return -1;
    }

    // help me, obi wan optimizer, you are my only hope
    if(table->table[slot] != NULL) {
        if(table->table[slot]->key != NULL) {
            // printf("cannot store duplicate key: \"%s\"\n", key);
            return 0;
        }
    }
    else {
        table->table[slot] = _ALLOC_TYPE(_hash_node_t);
    }

    table->table[slot]->key = _COPY_STRING(key);
    table->table[slot]->data = data;
    table->count++;

    // printf("inserted key: %s:%p in %p\n", key, (void*)data, (void*)table);
    return 1;
}

int find_hashtable(hash_table_t* tab, const char* key, void** data) {

    *data = NULL;
    int slot = find_slot(tab, key);

    if(tab->table[slot] != NULL && tab->table[slot]->key != NULL) {
        if(strcmp(tab->table[slot]->key, key) == 0) {
            *data = tab->table[slot]->data;
            // printf("key = %s\n", key);
            return 1;
        }
    }

    return 0;
}

void remove_hashtable(hash_table_t* tab, const char* key) {

    int slot = find_slot(tab, key);

    if((tab->table[slot] != NULL) && (tab->table[slot]->key != NULL)) {
        if(strcmp(tab->table[slot]->key, key) == 0) {
            _FREE(tab->table[slot]->key);
            tab->table[slot]->key = NULL;
            tab->count--;
            tab->tombstones++;
        }
    }
}

void dump_hashtable(hash_table_t* tab) {

    int count = 1;

    printf("cap = %d\n", tab->cap);
    printf("count = %d\n", tab->count);
    for(int i = 0; i < tab->cap; i++) {
        if(tab->table[i] != NULL) {
            if(tab->table[i]->key != NULL) {
                printf("%3d. slot=%d key=%s\n", count, i, tab->table[i]->key);
                count++;
            }
        }
    }
}

int hash_name_exists(hash_table_t* tab, const char* key) {

    int slot = find_slot(tab, key);

    if(tab->table[slot] != NULL && tab->table[slot]->key != NULL) {
        if(strcmp(tab->table[slot]->key, key) == 0) {
            return 1;
        }
    }

    return 0;
}


/*
 * Testing the hash table
 */
#ifdef TEST_HASH

const char* get_file_name(void) {
    return "nofile";
}
int get_line_no(void) {
    return -1;
}
int get_col_no(void) {
    return -1;
}


const char* slist[] = { "asdf", "1234", "weiuyer", "asdasd", "oiuoiu", "098098",
                        "(*&(*&", "}{P}{P}{", "KSDKJH", "OIUO***&*", NULL };

int main(void) {

    hash_table_t* tab = create_hashtable();

    for(int i = 0; slist[i] != NULL; i++)
        insert_hashtable(tab, slist[i], (void*)slist[i]);

    void* str;
    int num = 0;

    for(num = 0; slist[num] != NULL; num++) {
        if(find_hashtable(tab, slist[num], &str)) {
            printf("find: slist[%d] = %s = %s\n", num, slist[num], (const char*)str);
        }
    }

    insert_hashtable(tab, "pookey", NULL);
    if(find_hashtable(tab, "pookey", &str)) {
        printf("find: slist[\"pookey\"] = %s\n", (const char*)str ? "data" : "NULL");
    }
    else {
        printf("pookey not found\n");
    }

    if(find_hashtable(tab, "snark", &str)) {
        printf("find: slist[\"snark\"] = %s\n", (const char*)str ? "data" : "NULL");
    }
    else {
        printf("snark not found\n");
    }


    dump_hashtable(tab);
    destroy_hashtable(tab);
}

#endif
