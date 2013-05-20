#include <stdlib.h>
#include <string.h>
#include "objects.h"

ioobject_t new_object(ioobject_t proto){
    ioobject_t ret = malloc(sizeof *ret);
    ret->proto = proto;
    ret->activate_norecursion = 0;
    return ret;
}

ioobject_t hashmap_find(iohashmap_t hm, char *what){
    iohashmap_t cur;
    for(cur = hm; cur != NULL; cur = cur->next){
        if(!strcmp(cur->key, what))
            return cur->value;
    }
    return NULL;
}

ioobject_t find_attribute(ioobject_t ofobj, char *name){
    ioobject_t ret;
    ret = hashmap_find(ofobj->content.slots, name);
    if (ret == NULL && ofobj->proto != NULL){
        ret = find_attribute(ofobj->proto, name);
    }
    return ret;
}

const ioobject_t STRING_OBJ = NULL;//XXX

ioobject_t new_string(char *string){
    ioobject_t ret = new_object(STRING_OBJ);
    ret->type = BUFFER;
    ret->content.buffer = malloc(sizeof(ret->content.buffer) +
            strlen(string));
    strcpy(ret->content.buffer->data, string);
    return ret;
}

const ioobject_t INT_OBJ = NULL; //XXX

ioobject_t new_integer(int integer){
    ioobject_t ret = new_object(INT_OBJ);
    ret->type = WORD;
    ret->content.word = integer;
    return ret;
}

ioobject_t activate_slot(ioobject_t target, ioobject_t owner, iomessage_t msg){
    ioobject_t activator = NULL;
    if (target->type == CFUNC) {
        return target->content.function(owner, msg);
    } 
    if (target->type != OBJECT){
        target = target->proto;
    }

    activator = find_attribute(target, "activate");
    if (!activator || target->activate_norecursion) return target;
    else {
        ioobject_t ret = NULL;
        target->activate_norecursion = 1;
        ret =  activate_slot(activator, target, msg);
        target->activate_norecursion = 0;
        return ret;
    }
}

ioobject_t send_message(ioobject_t self, iomessage_t msg){
    ioobject_t target = NULL;
    ioobject_t findin = NULL;

    if (msg->type == CONSTSTR){
        return new_string(msg->value.string);
    } else if (msg->type == CONSTINT){
        return new_integer(msg->value.integer);
    } 

    if (self->type == OBJECT) {
        findin = self;
    } else {
        findin = self->proto;
    }
    target = find_attribute(findin, msg->value.symbol);
    if(!target){
        target = find_attribute(findin, "not_found");
    }
    return activate_slot(target, self, msg);
}
