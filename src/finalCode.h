#include "utilities.h"

enum vmopcode {
    assign_v,
    add_v,
    sub_v,
    mul_v,
    divide_v,
    mod_v,
    uminus_v,
    
    and_v,
    or_v,
    not_v,
    
    newtable_v,
    tablegetelem_v,
    tablesetelem_v,
    
    nop_v,
    
    jump_v,
    jeq_v,
    jne_v,
    jgt_v,
    jge_v,
    jlt_v,
    jle_v,
    
    enterfunc_v,
    exitfunc_v,
    callfunc_v,
    pusharg_v,
};


enum vmarg_t  {
    label_a,
    
    global_a,
    local_a,
    formal_a,

    userfunc_a,
    libfunc_a,

    number_a,
    bool_a,
    string_a,

    retval_a,

    nil_a,
};

typedef struct vmarg {
    enum vmarg_t type;
    unsigned val;
} vmArg;

typedef struct instruction {
    enum vmopcode opcode;
    vmArg* result;
    vmArg* arg1;
    vmArg* arg2;
    unsigned srcLine;
    unsigned t_address;
} instruction;

#define FINAL_EXPAND_SIZE 1024
#define FINAL_CURR_SIZE (totalFinal * sizeof(instruction))
#define FINAL_NEW_SIZE (EXPAND_SIZE * sizeof(instruction) + FINAL_CURR_SIZE)

typedef void (*generator_func_t)(quad *);

unsigned consts_newstring(char* s);
unsigned consts_newnumber(double n);
unsigned libfuncs_newused(char* s);
unsigned userfuncs_newfunc(SymbolTableEntry* sym);

void make_operand(Expr* e, vmArg* arg);

void make_number_operand(vmArg* arg, double val);

void make_bool_operand(vmArg* arg, unsigned val);

void make_retval_operand(vmArg* arg);

void expandFinal(void);

void emitFinalQuad(instruction *t);

void generateFinalCode(void);

void generate(enum vmopcode op, quad* q);

void generate_ADD(quad* q);
void generate_SUB(quad* q);
void generate_MUL(quad* q);
void generate_DIV(quad* q);
void generate_MOD(quad* q);

void printFinalQuads(void);

char* getVmOpcode(instruction* t);
