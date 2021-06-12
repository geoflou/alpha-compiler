#include "finalCode.h"

instruction *finalQuads = (instruction *)0;
unsigned totalFinal = 0;
unsigned currFinalQuad = 0;

//NOTE: H seira prepei na einai opws to enum iopcode
//      sto utilities.h
generator_func_t generators[] = {

};

void expandFinal(void) {
    assert(totalFinal == currFinalQuad);
    instruction *p = (instruction *)malloc(FINAL_NEW_SIZE);
    if (finalQuads)
    {
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
    i = t;

    return;
}

void generate(void) {
    unsigned i;
    quad* quads_temp;
    quads_temp = getQuads();
    for(i = 0; i < getcurrQuad(); ++i) {
        (*generators[(quads_temp + i)->op]) (quads_temp + i);
    }

    
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
