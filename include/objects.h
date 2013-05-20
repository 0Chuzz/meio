#ifndef __OBJECTS_H
#define __OBJECTS_H
#include "parser.h"

typedef struct _ioobject *ioobject_t;
typedef struct _iohashmap *iohashmap_t;

typedef struct _iohashmap { //XXX
    char *key;
    ioobject_t value;
    iohashmap_t next;
} *iohashmap_t;

typedef struct _ioprimitive_buffer{
    ioobject_t proto;
    unsigned int length;
    char data[];
} *ioprimitive_buffer_t;

typedef struct _ioobject {
    ioobject_t proto;
    enum ioslot_type {
        OBJECT,
        CFUNC,
        BUFFER,
        WORD,
        POINTER
    } type;
    union {
        iohashmap_t slots;
        ioobject_t (*function) (ioobject_t, iomessage_t);
        ioprimitive_buffer_t buffer;
        long word;
        void *ptr;
    } content;
    int activate_norecursion;
} *ioobject_t;

#endif
