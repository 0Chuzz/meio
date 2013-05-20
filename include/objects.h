#ifndef __OBJECTS_H
#define __OBJECTS_H

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

typedef struct _ioprimitive_arglist {
    enum argtype {
        BYVAL,
        BYPTR
    } type;
    ioprimitive_arglist_t next;
} *ioprimitive_arglist_t;

typedef struct _ioprimitive_code {
    ioobject_t proto;
    void (*function) ();
    ioprimitive_arglist_t arguments;
} *ioprimitive_code_t;

typedef struct _ioprimitive_data{
    ioobject_t proto;
    unsigned int length;
    char data[];
} *ioprimitive_data_t;

typedef struct _ioslot {
    enum ioslot_type {
        OBJECT,
        CODE,
        DATA
    } type;
    union {
        ioobject_t object;
        ioprimitive_code_t code;
        ioprimitive_data_t data;
    } content;
} *ioslot_t;

#endif
