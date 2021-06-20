#include "finalCode.h"

instruction *finalQuads = (instruction *) 0;
unsigned totalFinal = 0;
unsigned currFinalQuad = 0;

constString *constStringsArray = (constString *) 0;
constNum *constNumsArray = (constNum *) 0;
libFunc* libFuncs = (libFunc *) 0;
userFunc* userFuncs = (userFunc *) 0;

//NOTE: H seira prepei na einai opws to enum iopcode
//      sto utilities.h
generator_func_t generators[] = {
    generate_ADD,
    generate_SUB,
    generate_MUL,
    generate_DIV,
    generate_MOD,
    generate_ASSIGN,
    generate_IF_EQ,
    generate_IF_NOTEQ,
    generate_IF_LESSEQ,
    generate_IF_GREATEREQ,
    generate_IF_LESS,
    generate_IF_GREATER,
    generate_CALL,
    generate_PARAM,
    generate_RETURN,
    generate_GETRETVAL,
    generate_FUNCSTART,
    generate_FUNCEND,
    generate_NEWTABLE,
    generate_TABLEGETELEM,
    generate_TABLESETELEM,
    generate_JUMP
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

//NOTE: Maybe change t_address as lectures (Lecture 14, slide 17)
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

void generate_relational(enum vmopcode op, quad* q) {
    instruction* t;

    t = (instruction*)malloc(sizeof(instruction));
    t -> arg1 = (vmArg*)malloc(sizeof(vmArg));
    t -> arg2 = (vmArg*)malloc(sizeof(vmArg));
    t -> result = (vmArg*)malloc(sizeof(vmArg));

    t -> opcode = op;
    make_operand(q -> arg1, t -> arg1);
    make_operand(q -> arg2, t -> arg2);
    t -> result -> type = label_a;
    t -> result -> val = q -> label;
    t -> t_address = nextInstructionLabel() + 1;


    emitFinalQuad(t);
}

void make_operand(Expr* e, vmArg* arg) {
    
    if(e == NULL) {
        arg -> type = nil_a;
        return;
    }

    switch(e -> exprType) {
        case var_e:
        case tableitem_e:
        case arithexpr_e:
        case assignexpr_e:
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
            arg -> val = consts_newstring(e -> strConst);
            arg -> type = string_a;
            break;
        }

        case constnum_e: {
            arg -> val = consts_newnumber(e -> numConst);
            arg -> type = number_a;
            break;
        }

        case nil_e: {
            arg -> type = nil_a;
            break;
        }

        case programfunc_e: {
            arg -> type = userfunc_a;
            arg -> val = userfuncs_newfunc(e -> symbol);
            break;
        }

        case libraryfunc_e: {
            arg -> type = libfunc_a;
            arg -> val = libfuncs_newused(getEntryName(e -> symbol));
            break;
        }

        default: assert(0);
    }
}

void make_number_operand(vmArg* arg, double val) {
    arg -> val = consts_newnumber(val);
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

void generate_NEWTABLE(quad* q) {
    generate(newtable_v, q);
}

void generate_TABLEGETELEM(quad* q) {
    generate(tablegetelem_v, q);
}

void generate_TABLESETELEM(quad* q) {
    generate(tablesetelem_v, q);
}

void generate_ASSIGN(quad* q) {
    generate(assign_v, q);
}

void generate_NOP(void) {
    instruction *t = (instruction*) malloc(sizeof(instruction));
    t -> opcode = nop_v;
    emitFinalQuad(t);
}

void generate_PARAM(quad* q) {
    instruction* t;

    t = (instruction *)malloc(sizeof(instruction));
    t -> arg1 = (vmArg*)malloc(sizeof(vmArg));
    t -> arg2 = (vmArg*)malloc(sizeof(vmArg));
    t -> result = (vmArg*)malloc(sizeof(vmArg));

    t -> opcode = pusharg_v;
    make_operand(q -> arg1, t -> arg1);
    t -> t_address = nextInstructionLabel() + 1;
    emitFinalQuad(t);
}

void generate_CALL(quad * q) {
    instruction* t;

    t = (instruction *)malloc(sizeof(instruction));
    t -> arg1 = (vmArg*)malloc(sizeof(vmArg));
    t -> arg2 = (vmArg*)malloc(sizeof(vmArg));
    t -> result = (vmArg*)malloc(sizeof(vmArg));
    
    t -> opcode = callfunc_v;
    make_operand(q -> arg1, t -> arg1);
    t -> t_address = nextInstructionLabel() + 1;
    emitFinalQuad(t);
}

void generate_GETRETVAL(quad * q) {
    instruction *t;

    t = (instruction *)malloc(sizeof(instruction));
    t -> arg1 = (vmArg*)malloc(sizeof(vmArg));
    t -> arg2 = (vmArg*)malloc(sizeof(vmArg));
    t -> result = (vmArg*)malloc(sizeof(vmArg));

    t -> opcode = assign_v;
    make_operand(q -> result, t -> result);
    make_retval_operand(t -> arg1);
    emitFinalQuad(t);
}

void generate_JUMP(quad* q) {
    generate_relational(jump_v, q);
}

void generate_IF_EQ(quad* q) {
    generate_relational(jeq_v, q);
}

void generate_IF_NOTEQ(quad* q) {
    generate_relational(jne_v, q);
}

void generate_IF_GREATER(quad* q) {
    generate_relational(jgt_v, q);
}

void generate_IF_GREATEREQ(quad* q) {
    generate_relational(jge_v, q);
}

void generate_IF_LESS(quad* q) {
    generate_relational(jlt_v, q);
}

void generate_IF_LESSEQ(quad* q) {
    generate_relational(jle_v, q);
}

void generate_RETURN(quad* q) {
    instruction *t;

    t = (instruction *)malloc(sizeof(instruction));
    t -> arg1 = (vmArg*)malloc(sizeof(vmArg));
    t -> arg2 = (vmArg*)malloc(sizeof(vmArg));
    t -> result = (vmArg*)malloc(sizeof(vmArg));

    t -> opcode = assign_v;
    t -> t_address = nextInstructionLabel() + 1;
    make_retval_operand(t -> result);
    make_operand(q -> arg1, t -> arg1);
    emitFinalQuad(t);
}

void generate_FUNCSTART(quad *q) {
    instruction *t;
    userfuncs_newfunc(q -> result -> symbol);

    t = (instruction*)malloc(sizeof(instruction));
    t -> arg1 = (vmArg*)malloc(sizeof(vmArg));
    t -> arg2 = (vmArg*)malloc(sizeof(vmArg));
    t -> result = (vmArg*)malloc(sizeof(vmArg));

    t -> opcode = enterfunc_v;
    t -> t_address = nextInstructionLabel() + 1;
    make_operand(q -> result, t -> result);
    emitFinalQuad(t);
}

void generate_FUNCEND(quad *q) {
    instruction* t;
    
    t = (instruction*)malloc(sizeof(instruction));
    t -> arg1 = (vmArg*)malloc(sizeof(vmArg));
    t -> arg2 = (vmArg*)malloc(sizeof(vmArg));
    t -> result = (vmArg*)malloc(sizeof(vmArg));

    t -> opcode = exitfunc_v;
    t -> t_address = nextInstructionLabel() + 1;
    make_operand(q -> result, t -> result);
    emitFinalQuad(t);
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

    //printList();
}

unsigned consts_newstring(char* s) {
    constString* newNode;
    constString* index;

    newNode = (constString *)malloc(sizeof(constString));
    newNode -> value = s;
    newNode -> next = NULL;

    if(constStringsArray == NULL) {
        constStringsArray = newNode;
        return 0;
    }

    index = constStringsArray;
    while(index -> next != NULL) {
        index = index -> next;
    }

    index -> next = newNode;
    return 0;

}

unsigned consts_newnumber(double n) {
    constNum* newNode;
    constNum* index;

    newNode = (constNum *)malloc(sizeof(constNum));
    newNode -> value = n;
    newNode -> next = NULL;

    if(constNumsArray == NULL) {
        constNumsArray = newNode;
        return 0;
    }

    index = constNumsArray;
    while(index -> next != NULL) {
        index = index -> next;
    }

    index -> next = newNode;
    return 0;

}

unsigned libfuncs_newused(char* s) {
    libFunc* newNode;
    libFunc* index;

    newNode = (libFunc *)malloc(sizeof(libFunc));
    newNode -> name = s;
    newNode -> next = NULL;

    if(libFuncs == NULL) {
        libFuncs = newNode;
        return 0;
    }

    index = libFuncs;
    while(index -> next != NULL) {
        index = index -> next;
    }

    index -> next = newNode;
    return 0;

}

unsigned userfuncs_newfunc(SymbolTableEntry* sym) {
    assert(sym != NULL);
    
    userFunc* newNode;
    userFunc* index;

    newNode = (userFunc *)malloc(sizeof(userFunc));
    newNode -> name = getEntryName(sym);
    newNode -> totalLocals = sym -> funcVal -> totalLocalVars;
    newNode -> formalCount = sym -> funcVal -> argsCount;
    newNode -> address = nextInstructionLabel() + 1;
    newNode -> next = NULL;

    if(userFuncs == NULL) {
        userFuncs = newNode;
        return 0;
    }

    index = userFuncs;
    while(index -> next != NULL) {
        index = index -> next;
    }

    index -> next = newNode;
    return 0;
}

void printList(void) {
    constString* index;
    int i = 0;

    index = constStringsArray;
    while(index != NULL) {
        printf("%d \t %s\n", i , index -> value);
        i++;
        index = index -> next;
    }

    return;
}

void createBinaryFile(void) {
    FILE* file;
    unsigned int i = 0;
    unsigned tempStrlen;
    instruction *index;
    constNum *numIndex;
    constString *stringIndex;
    userFunc* funcIndex;
    unsigned offset;

    file = fopen("./code.out", "wb");
    if(!file) {
        printf("ERROR: file creation error!\n");
        return;
    }

    index = finalQuads;
    while(i < currFinalQuad) {
        fwrite(&i, sizeof(unsigned), 1, file);
        fwrite(&index -> opcode, sizeof(index -> opcode), 1, file);

        if(index -> result != NULL) {
            fwrite(&index -> result -> type, sizeof(index -> result), 1, file);
            if(index -> result -> type != retval_a) {
                fwrite(&index -> result -> val, sizeof(index -> result), 1, file);
            }
        }

        if(index -> arg1 != NULL) {
            fwrite(&index -> arg1 -> type, sizeof(index -> arg1), 1, file);
            if(index -> arg1 -> type != retval_a) {
                fwrite(&index -> arg1 -> val, sizeof(index -> arg1), 1, file);
            }
        }

        if(index -> arg2 != NULL) {
            fwrite(&index -> arg2 -> type, sizeof(index -> arg2), 1, file);
            if(index -> arg2 -> type != retval_a) {
                fwrite(&index -> arg2 -> val, sizeof(index -> arg2), 1, file);
            }
        }

        fwrite(&index -> srcLine, sizeof(unsigned), 1 , file);
        i++;
        index = finalQuads + i;
    }

    numIndex = constNumsArray;
    i = 0;
    while(numIndex != NULL) {
        fwrite(&i, sizeof(unsigned), 1, file);
        fwrite(&numIndex -> value, sizeof(double), 1, file);
        numIndex = numIndex -> next;
        i++;
    }

    stringIndex = constStringsArray;
    i = 0;
    while(stringIndex != NULL) {
        fwrite(&i, sizeof(unsigned), 1, file);
        tempStrlen = strlen(stringIndex -> value);
        fwrite(&tempStrlen, sizeof(unsigned), 1 , file);
        fwrite(&stringIndex -> value, sizeof(char)* tempStrlen, 1, file);
        stringIndex = stringIndex -> next;
        i++;
    }

    funcIndex = userFuncs;
    i = 0;
    while(funcIndex != NULL) {
        fwrite(&i, sizeof(unsigned), 1, file);
        fwrite(&funcIndex -> address, sizeof(unsigned), 1, file);
        fwrite(&funcIndex -> totalLocals, sizeof(unsigned), 1, file);
        fwrite(&funcIndex -> formalCount, sizeof(unsigned), 1, file);
        tempStrlen = strlen(funcIndex -> name);
        fwrite(&tempStrlen, sizeof(unsigned), 1 , file);
        fwrite(&funcIndex -> name, sizeof(char)* tempStrlen, 1, file);
        funcIndex = funcIndex -> next;
        i++;
    }

    offset = getGlobalOffset();
    fwrite(&offset, sizeof(unsigned), 1 , file);
    fclose(file);
}