#ifndef REGISTERS
#define REGISTERS

typedef struct reg_s{
    char* name;
    struct reg_s* next; 
} reg_t;

typedef struct {
    reg_t* top;
} reg_stack;

void initialize_registers();
void free_registers();

//Stack operations
reg_t* reg_pop();
void reg_push(reg_t* r);
void reg_swap();

#endif