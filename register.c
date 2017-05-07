#include <stdio.h>
#include <stdlib.h>
#include "register.h"

reg_stack rst;

char registers[][5] = {
	"%r10",
	"%r11",
	"%r12",
	"%r13",
	"%r14",
	"%r15",
};

void initialize_registers(){
    rst.top = NULL;
    reg_t* r;

    for(int i=5; i >= 0; i--){
        r = rst.top;
        rst.top = malloc(sizeof(reg_t));
        rst.top->name = registers[i];
        rst.top->next = r;
    }
    
    return;
}

void free_registers(){
    reg_t* r, * temp;
    for(r = rst.top; r != NULL;){
        temp = r;
        r = r->next;
        free(temp);
    }
}

reg_t* reg_pop(){
    reg_t* r;

    r = rst.top;
    rst.top = rst.top->next;
    return r;
}

void reg_push(reg_t* r){
    reg_t* t;

    t = rst.top;
    rst.top = r;
    r->next = t;

    return;
}

void reg_swap(){
    reg_t* r;
    r = rst.top->next;
    rst.top->next = r->next;
    r->next = rst.top;
    rst.top = r;
    return;
}