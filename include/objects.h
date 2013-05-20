#ifndef __OBJECTS_H
#define __OBJECTS_H
#include "parser.h"

typedef struct _ioobject *ioobject_t;
typedef struct _ioslot *ioslot_t;
typedef struct _ioprimitive_arglist *ioprimitive_arglist_t;
typedef struct _iohashmap *iohashmap_t;

typedef struct _iohashmap { //XXX
    char *key;
    ioslot_t value;
    iohashmap_t next;
} *iohashmap_t;

typedef struct _ioobject {
    ioobject_t proto;
    iohashmap_t slots;
} *ioobject_t;

typedef struct _ioprimitive_code {
    ioobject_t proto;
    ioslot_t (*function) (ioslot_t, iomessage_t);
} *ioprimitive_code_t;

typedef struct _ioprimitive_buffer{
    ioobject_t proto;
    unsigned int length;
    char data[];
} *ioprimitive_buffer_t;

typedef struct _ioprimitive_long{
    ioobject_t proto;
    long value;
} *ioprimitive_long_t;

typedef struct _ioprimitive_ptr{
    ioobject_t proto;
    void *value;
} *ioprimitive_ptr_t;

typedef struct _ioslot {
    enum ioslot_type {
        OBJECT,
        CFUNC,
        BUFFER,
        WORD,
        POINTER
    } type;
    union {
        ioobject_t object;
        ioprimitive_code_t cfunc;
        ioprimitive_long_t buffer;
        ioprimitive_ptr_t word;
    } content;
    int activate_norecursion;
} *ioslot_t;

#endif
