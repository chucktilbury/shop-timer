
#ifndef _ALLOC_H_
#define _ALLOC_H_

#include <stddef.h>

#define _ALLOC(s) _mem_alloc(s)
#define _ALLOC_TYPE(t) (t*)_mem_alloc(sizeof(t))
#define _ALLOC_ARRAY(t, n) (t*)_mem_alloc(sizeof(t) * (n))
#define _REALLOC(p, s) _mem_realloc((void*)(p), (s))
#define _REALLOC_ARRAY(p, t, n) (t*)_mem_realloc((void*)(p), sizeof(t) * (n))
#define _COPY(p, s) _mem_copy((void*)(p), (s))
#define _COPY_TYPE(p, t) (t*)_mem_copy((void*)(p), sizeof(t))
#define _COPY_ARRAY(p, t, n) (t*)_mem_copy((void*)(p), sizeof(t) * (n))
#define _COPY_STRING(s) _mem_copy_string(s)
#define _FREE(p) _mem_free((void*)(p))

void* _mem_alloc(size_t);
void* _mem_realloc(void*, size_t);
void* _mem_copy(void*, size_t);
char* _mem_copy_string(const char*);
void _mem_free(void*);

#endif /* _ALLOC_H_ */
