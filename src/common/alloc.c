
#include <stdlib.h>
#include <string.h>

#include "errors.h"

#ifdef USE_GC
#include "gc.h"
#define _MALLOC GC_malloc
#define _REALLOC GC_realloc
#define _FREE GC_free
#else
#define _MALLOC malloc
#define _REALLOC realloc
#define _FREE free
#endif

void* _mem_alloc(size_t size) {

    void* ptr = _MALLOC(size);
    if(ptr == NULL)
        FATAL("cannot allocate %lu bytes", size);

    memset(ptr, 0, size);
    return ptr;
}

void* _mem_realloc(void* optr, size_t size) {

    void* nptr = _REALLOC(optr, size);
    if(nptr == NULL)
        FATAL("cannot re-allocate %lu bytes", size);

    return nptr;
}

void* _mem_copy(void* optr, size_t size) {

    void* nptr = _MALLOC(size);
    if(nptr == NULL)
        FATAL("cannot allocate to copy %lu bytes", size);

    memcpy(nptr, optr, size);
    return nptr;
}

char* _mem_copy_string(const char* str) {

    size_t len;
    if(str != NULL)
        len = strlen(str) + 1;
    else
        len = 1;

    char* ptr = _MALLOC(len);
    if(ptr == NULL)
        FATAL("cannot allocate %lu bytes for string", len);

    if(str != NULL)
        memcpy(ptr, str, len);
    else
        ptr[0] = '\0';

    return ptr;
}

void _mem_free(void* ptr) {

    if(ptr != NULL)
        _FREE(ptr);
}
