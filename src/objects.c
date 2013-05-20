#include <stdlib.h>
#include <string.h>
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

ioslot_t new_primitive_data(ioobject_t proto, size_t size){
    ioprimitive_data_t ret = malloc((sizeof *ret) + size);
    ret->proto = proto;
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

ioslot_t call_primitive(ioprimitive_code_t code, 
        ioslot_t slot, iomessage_t msg){
    return code->function(slot, msg);
}

ioslot_t activate_slot(ioslot_t target, ioslot_t owner, iomessage_t msg){
    ioslot_t activator = NULL;
    if (target->type == CODE) {
        return call_primitive(target->content.code, owner, msg);
    } 
    if (target->type != OBJECT){
        target = target->content.data->proto;
    }
    //XXX probably won't work like this
    activator = find_attribute(target, "activate");
    if (!activator || target->activate_norecursion) return target;
    else {
        ioslot_t ret = NULL;
        target->activate_norecursion = 1;
        iomessage_t activmsg = new_message();
        activmsg->name = "activate";
        activmsg->type = SYMBOL;
        activmsg->arguments= new_arg_list();
        activmsg->arguments->argument = msg;

        ret =  send_message(target, activmsg);
        target->activate_norecursion = 0;
        return ret;
    }
}

ioslot_t send_message(ioslot_t self, iomessage_t msg){
    ioslot_t target = NULL;
    ioobject_t findin = NULL;

    if (msg->type == CONSTSTR){
        return new_string(msg->value.string);
    } else if (msg->type == CONSTINT){
        return new_integer(msg->value.integer);
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
    return activate_slot(target, self, msg);
}
