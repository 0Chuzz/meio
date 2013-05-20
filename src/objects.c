#include <stdlib.h>
#include "parser.h"
#include "objects.h"

ioobject_t new_object(ioobject_t proto){
    ioobject_t ret = malloc(sizeof *ret);
    ret->proto = proto;
    ret->slots = NULL;
    return ret;
}

ioslot_t hashmap_find(iohashmap_t hm, char *what){
    iohashmap_t cur;
    for(cur = hm; cur != NULL; cur = cur->next){
        if(!strcmp(cur->key, what))
            return cur->value;
    }
    return NULL;
}

ioslot_t find_attribute(ioobject_t ofobj, char *name){
    ioslot_t ret;
    ret = hashmap_find(ofobj->slots, name);
    if (ret == NULL && ofobj->proto != NULL){
        ret = find_attribute(ofobj->proto, name);
    }
    return ret;
}


ioslot_t new_string(char *string){
    ioslot_t ret = new_primitive_data(STRING_OBJ, strlen(string));
    strcpy(ret->content.data->data, string);
    return ret;
}

ioslot_t new_integer(int integer){
    ioslot_t ret = new_primitive_data(INT_OBJ, sizeof integer);
    int *writeto = (int *) (ret->content.data->data);
    *writeto = integer;
    return ret;
}

ioslot_t send_message(ioslot_t self, iomessage_t msg){
    ioslot_t target = NULL;
    ioobject_t findin = NULL;

    if (msg->type == CONSTSTR){
        return new_string(msg->value.string);
    } else if (msg->type == CONSTINT){
        return new_integer(msg->value.integer);
    } 

    if (self->type == CODE) {
        return call_primitive(self->content.code, msg->arguments);
    } 
    if (self->type == OBJECT) {
        findin = self->content.object;
    } else {
        findin = self->content.data->proto;
    }
    target = find_attribute(findin, msg->value.symbol);
    if(!target){
        target = find_attribute(findin, "not_found");
    }
    return NULL; // activate_slot(target, self, msg);
}
