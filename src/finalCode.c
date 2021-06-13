#include "finalCode.h"

instruction *finalQuads = (instruction *) 0;
unsigned totalFinal = 0;
unsigned currFinalQuad = 0;

//NOTE: H seira prepei na einai opws to enum iopcode
//      sto utilities.h
generator_func_t generators[] = {
    generate_ADD,
    generate_SUB,
    generate_MUL,
    generate_DIV,
    generate_MOD
};

void expandFinal(void) {
    assert(totalFinal == currFinalQuad);
    instruction *p = (instruction *)malloc(FINAL_NEW_SIZE);
    if (finalQuads) {
        memcpy(p, finalQuads, FINAL_CURR_SIZE);
        free(finalQuads);
    }

    finalQuads = p;
    totalFinal += FINAL_EXPAND_SIZE;

    return;
}

void emitFinalQuad(instruction *t) {
    if(currFinalQuad == totalFinal) {
        expandFinal();
    }
    
    instruction *i = finalQuads + currFinalQuad++;
    memcpy(i, t, sizeof(instruction));
    free(t);
    

    return;
}

unsigned nextInstructionLabel(void) {
    return currFinalQuad;
}

void generateFinalCode(void) {
    unsigned i;
    quad* quads_temp = (quad*)malloc(sizeof(quad));
    quads_temp = getQuads();
    for(i = 0; i < getcurrQuad(); ++i) {
        (*generators[(quads_temp + i) -> op]) ((quads_temp + i));
    }

}

//NOTE: Change t_address as lectures (Lecture 14, slide 17)
void generate(enum vmopcode op, quad* q) {
    instruction *t = (instruction *)malloc(sizeof(instruction));

    t -> arg1 = (vmArg*)malloc(sizeof(vmArg));
    t -> arg2 = (vmArg*)malloc(sizeof(vmArg));
    t -> result = (vmArg*)malloc(sizeof(vmArg));

    t -> opcode = op;
    make_operand(q -> arg1, t -> arg1);
    make_operand(q -> arg2, t -> arg2);
    make_operand(q -> result, t -> result);
    t -> t_address = nextInstructionLabel() + 1;
    t -> srcLine = q -> line;

    emitFinalQuad(t);
}

void make_operand(Expr* e, vmArg* arg) {
    assert(e != NULL);
    switch(e -> exprType) {
        case var_e:
        case tableitem_e:
        case arithexpr_e:
        case boolexpr_e:
        case newtable_e: {
            assert(e -> symbol != NULL);
            arg -> val = e -> symbol -> offset;
            switch (e -> symbol -> scopespace) {
                case programvar: {
                    arg -> type = global_a;
                    break;
                }
                case functionlocal: {
                    arg -> type = local_a;
                    break;
                }
                case formalarg: {
                    arg -> type = formal_a;
                    break;
                }

                default: assert(0);
            }

            break;
        }

        case constbool_e: {
            arg -> val = e -> boolConst;
            arg -> type = bool_a;
            break;
        }

        case conststring_e: {
            //arg -> val = consts_newstring(e -> strConst);
            arg -> type = string_a;
            break;
        }

        case constnum_e: {
            //arg -> val = consts_newnumber(e -> numConst);
            arg -> type = number_a;
            break;
        }

        case nil_e: {
            arg -> type = nil_a;
            break;
        }

        case programfunc_e: {
            arg -> type = userfunc_a;
            //arg -> val = userfuncs_newfunc(e -> symbol);
            break;
        }

        case libraryfunc_e: {
            arg -> type = libfunc_a;
            //arg -> val = libfuncs_newused(getEntryName(e -> symbol));
            break;
        }

        default: assert(0);
    }
}

void make_number_operand(vmArg* arg, double val) {
    //arg -> val = consts_newnumber(val);
    arg -> type = number_a;
}

void make_bool_operand(vmArg* arg, unsigned val) {
    arg -> val = val;
    arg -> type = bool_a;
}

void make_retval_operand(vmArg* arg) {
    arg -> type = retval_a;
}

void generate_ADD(quad* q) {
    generate(add_v, q);
}

void generate_SUB(quad* q) {
    generate(sub_v, q);
}

void generate_MUL(quad* q) {
    generate(mul_v, q);
}

void generate_DIV(quad* q) {
    generate(divide_v, q);
}

void generate_MOD(quad* q) {
    generate(mod_v, q);
}

char* getVmOpcode(instruction* t) {
    assert(t != NULL);

    switch(t -> opcode) {
        case assign_v: {
            return "assign";
        }
        case add_v: {
            return "add";
        }
        case sub_v: {
            return "sub";
        }
        case mul_v: {
            return "mul";
        }
        case divide_v: {
            return "div";
        }
        case mod_v: {
            return "mod";
        }
        case uminus_v: {
            return "uminus";
        }
        case and_v: {
            return "and";
        }
        case or_v: {
            return "or";
        }
        case not_v: {
            return "not";
        }
        case newtable_v: {
            return "newtable";
        }
        case tablegetelem_v: {
            return "tablegetelem";
        }
        case tablesetelem_v: {
            return "tablesetelem";
        }
        case nop_v: {
            return "nop";
        }
        case jump_v: {
            return "jump";
        }
        case jeq_v: {
            return "jeq";
        }
        case jne_v: {
            return "jne";
        }
        case jgt_v: {
            return "jgt";
        }
        case jge_v: {
            return "jge";
        }
        case jlt_v: {
            return "jlt";
        }
        case jle_v: {
            return "jle_v";
        }
        case enterfunc_v: {
            return "enterfunc";
        }
        case exitfunc_v: {
            return "exitfunc";
        }
        case callfunc_v: {
            return "callfunc";
        }
        case pusharg_v: {
            return "pusharg";
        }
        default: assert(0);
    }
}

void printFinalQuads(void) {
    int i = 0;
    instruction* index;

    printf("\n\033[0;35mINSTR# \t OPCODE\033[0m\n");
    printf("===============================================\n");
    while(i < currFinalQuad) {
        index = finalQuads + i;
        printf("#%d \t %-10s\n", (i + 1), getVmOpcode(finalQuads + i));
        i++;
    }
    printf("===============================================\n");
}