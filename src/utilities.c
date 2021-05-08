#include "utilities.h"

quad *quads = (quad *)0;
unsigned total = 0;
unsigned currQuad = 0;

unsigned programVarOffset = 0;
unsigned functionLocalOffset = 0;
unsigned formalArgOffset = 0;
unsigned scopeSpaceCounter = 1;

unsigned tempCounter = 0;

void insertID(char *name, int scope, int line) {
    SymbolTableEntry *temp;
    SymbolTableEntry *new_entry;
    Variable *new_var;

    temp = lookupEverything(name, scope);

    if (temp == NULL) {
        new_entry = (SymbolTableEntry *)malloc(sizeof(SymbolTableEntry));
        new_var = (Variable *)malloc(sizeof(Variable));

        new_var->name = name;
        new_var->scope = scope;
        new_var->line = line;

        new_entry->isActive = 1;
        new_entry->varVal = new_var;
        new_entry->funcVal = NULL;
        new_entry->offset = currScopeOffset() + 1;
        incScopeOffset();

        scope == 0 ? (new_entry->type = GLOBAL) : (new_entry->type = LOCAL);

        insertEntry(new_entry);
        return;
    }
    if (getEntryType(temp) == "USERFUNC" && temp->isActive == 1) {
        printf("\033[31mERROR: A function has taken already that name!\033[0m");
    }

    return;
}

void insertLocalID(char *name, int scope, int line) {
    SymbolTableEntry *temp;
    SymbolTableEntry *new_entry;
    Variable *new_var;

    if (comparelibfunc(name) == -1) {
        printf("\033[31mERROR: Function name redefinition \"%s\" is a library function\033[0m", name);
        yyerror("\t");
        return;
    }

    temp = lookupScope(name, scope);

    if (temp == NULL) {
        new_entry = (SymbolTableEntry *)malloc(sizeof(SymbolTableEntry));
        new_var = (Variable *)malloc(sizeof(Variable));

        new_var->name = name;
        new_var->scope = scope;
        new_var->line = line;

        new_entry->isActive = 1;
        new_entry->varVal = new_var;
        new_entry->offset = currScopeOffset() + 1;
        incScopeOffset();
        new_entry->funcVal = NULL;

        scope == 0 ? (new_entry->type = GLOBAL) : (new_entry->type = LOCAL);

        insertEntry(new_entry);
        return;
    }

    if (temp->type == USERFUNC && temp->isActive == 1) {
        printf("\033[31mERROR: A user function with that name already exists!\033[0m\n");
    }

    return;
}

void insertFormal(char *name, int scope, int line) {
    SymbolTableEntry *temp;
    SymbolTableEntry *new_entry;
    Variable *new_var;

    assert(name != NULL);
    if (comparelibfunc(name) == -1) {
        printf("\033[31mERROR: Function name redefinition \"%s\" is a library function\033[0m", name);
        yyerror("\t");
        return;
    }

    new_entry = (SymbolTableEntry *)malloc(sizeof(SymbolTableEntry));
    new_var = (Variable *)malloc(sizeof(Variable));

    new_var->name = name;
    new_var->scope = scope;
    new_var->line = line;

    new_entry->isActive = 1;
    new_entry->varVal = new_var;
    new_entry->offset = currScopeOffset() + 1;
    incScopeOffset();
    new_entry->funcVal = NULL;
    new_entry->type = FORMAL;

    insertEntry(new_entry);
    return;
}

unsigned currScopeOffset(void) {
    if (scopeSpaceCounter == 1) {
        return programVarOffset;
    }

    if (scopeSpaceCounter % 2 == 0) {
        return formalArgOffset;
    }

    return functionLocalOffset;
}

void enterScopeSpace(void) {
    scopeSpaceCounter++;
    return;
}

void exitScopeSpace(void) {
    assert(scopeSpaceCounter > 1);
    --scopeSpaceCounter;
    return;
}

void expand(void) {
    assert(total == currQuad);
    quad *p = (quad *)malloc(NEW_SIZE);
    if (quads)
    {
        memcpy(p, quads, CURR_SIZE);
        free(quads);
    }

    quads = p;
    total += EXPAND_SIZE;

    return;
}

void emit(enum iopcode op, Expr *arg1, Expr *arg2, Expr *result, unsigned label, unsigned line) {
    if (currQuad == total) {
        expand();
    }

    quad *p = quads + currQuad++;
    p->op = op;
    p->arg1 = arg1;
    p->arg2 = arg2;
    p->result = result;
    p->label = label;
    p->line - line;

    return;
}

char *newTempName(void) {
    char *name = (char *)malloc(sizeof(char) * 10);
    sprintf(name, "_t%d", tempCounter);
    tempCounter++;
    return name;
}

SymbolTableEntry *newTemp(int scope, int line) {
    char *name = newTempName();
    Variable *newVar;
    SymbolTableEntry *sym = lookupScope(name, scope);

    if (sym != NULL) {
        return sym;
    }

    sym = (SymbolTableEntry *)malloc(sizeof(SymbolTableEntry));
    newVar = (Variable *)malloc(sizeof(Variable));

    newVar->line = line;
    newVar->name = name;
    newVar->scope = scope;

    sym->varVal = newVar;
    scope == 0 ? (sym->type = GLOBAL) : (sym->type = LOCAL);
    sym->isActive = 1;
    sym->funcVal = NULL;
    sym->next = NULL;

    insertEntry(sym);

    return sym;
}

Expr *lvalue_expr(SymbolTableEntry *sym, int scope, int line) {
    assert(sym != NULL);
    Expr *e = (Expr *)malloc(sizeof(Expr));
    memset(e, 0, sizeof(Expr)); //possible segfault

    e->next = (Expr *)0;
    e->symbol = sym;

    if (sym->type == USERFUNC) {
        e->exprType = programfunc_e;
        return e;
    }

    if (sym->type == LIBFUNC) {
        e->exprType = libraryfunc_e;
        return e;
    }

    if (sym->type == GLOBAL || sym->type == LOCAL || sym->type == FORMAL) {
        e->exprType = var_e;
        return e;
    }

    assert(0);
}

Expr *newExpr(enum expr_t type) {
    Expr *newEx = (Expr *)malloc(sizeof(Expr));
    newEx->exprType = type;
    return newEx;
}

Expr *newExpr_conststring(char *s) {
    Expr *e = newExpr(conststring_e);
    e->strConst = strdup(s);
    return e;
}

Expr *newExpr_constbool(unsigned char b) {
    Expr *e = newExpr(constbool_e);
    e->boolConst = b;
    return e;
}

Expr *newExpr_constnum(double s)
{
    Expr *e = newExpr(constnum_e);
    e->numConst = s;
    return e;
}

char *getOpCode(quad *q)
{
    assert(q != NULL);
    switch (q->op) {
        case assign:
            return "assign";

        case add:
            return "add";

        case sub:
            return "sub";

        case mul:
            return "mul";

        case divide:
            return "divide";

        case mod:
            return "mod";

        case uminus:
            return "uminus";

        case and:
            return "and";

        case or:
            return "or";

        case not:
            return "not";

        case if_eq:
            return "if_eq";

        case if_noteq:
            return "if_noteq";

        case if_lesseq:
            return "if_lesseq";

        case if_greatereq:
            return "if_greatereq";

        case if_less:
            return "if_less";

        case if_greater:
            return "if_greater";

        case call:
            return "call";

        case param:
            return "param";

        case ret:
            return "ret";

        case getretval:
            return "getretval";

        case funcstart:
            return "funcstart";

        case funcend:
            return "funcend";

        case tablecreate:
            return "tablecreate";

        case tablegetelem:
            return "tablegetelem";

        case tablesetelem:
            return "tablesetelem";

        case jump:
            return "jump";

        default:
            assert(0);
            break;
    }
}

char* getExpr(Expr* e) {
    char * toString = (char*)malloc(sizeof(char)* 100);
    
    if(e == NULL) {
        return " ";
    }
    if(e-> exprType == var_e || e-> exprType == programfunc_e || e-> exprType == libraryfunc_e) {
        return getEntryName(e->symbol);
    }

    if (e-> exprType == conststring_e) {
        sprintf(toString, "\"%s\"", e->strConst);
        return toString;
    }

    if (e-> exprType == constnum_e) {
        sprintf(toString, "%0.1f", e->numConst);
        return toString;
    }

    if(e-> exprType == constbool_e) {
        if (e->boolConst==0) {
                return "false";
        }

        return "true";
    }

    if(e -> exprType == arithexpr_e || e -> exprType == assignexpr_e || e -> exprType == boolexpr_e) {
        return getEntryName(e -> symbol);
    }
    if(e-> exprType == tableitem_e) {
        return getEntryName(e -> symbol);
    }

    if (e-> exprType == newtable_e) {
        return getEntryName(e -> symbol);
    }

    return "periptwsh";

}

void printQuads(void) {
    int i = 0;
    quad *index;
    printf("\033[0;35mQUAD# \t OPCODE \t \t RESULT \t ARG1 \t ARG2 \t LABEL\033[0m\n");
    printf("=============================================================================================\n");
    while(i < currQuad) {
        index = quads + i;
        printf("#%d \t %s \t\t %s \t\t %s \t %s \t %d \n", i, getOpCode(index), getExpr(index -> result), 
            getExpr(index -> arg1), getExpr(index -> arg2), index -> label);
        i++;
    }
    printf("\033[32;1mprogram var offset is : %d\033[0m\n", programVarOffset);
    printf("\033[32;1mlocal var offset is : %d\033[0m\n", functionLocalOffset);
    printf("\033[32;1mformal args offset is : %d\033[0m\n", formalArgOffset);
    
}


unsigned int getcurrQuad(void) {
    return currQuad;
}

void incScopeOffset(void) {

    if (scopeSpaceCounter == 1) {
        programVarOffset++;
        return;
    }

    if (scopeSpaceCounter % 2 == 0) {
        formalArgOffset++;
        return;
    }

    functionLocalOffset++;
    return;
}


void resetScopeOffset(void) {

    if (scopeSpaceCounter == 1) {
        programVarOffset = 0;
        return;
    }

    if (scopeSpaceCounter % 2 == 0) {
        formalArgOffset = 0;
        return;
    }

    functionLocalOffset = 0;
    return;
}

Expr * emit_ifTableItem(Expr* e, int scope, int line) {
    assert(e!=NULL);
    if(e->exprType != tableitem_e) {
        return e;
    }
    else {
        Expr* result = newExpr(var_e);
        result->symbol = newTemp(scope , line);
        emit(tablegetelem,e,e->index,result,getcurrQuad()+1, line);
        return result;
    }
}

Expr * member_item(Expr* e, char * name, int scope, int line) {
    e = emit_ifTableItem(e, scope, line);
    Expr* item = newExpr(tableitem_e);
    item -> symbol = e-> symbol;
    item -> index = newExpr_conststring(name);
    return item;
}

void deleteExprList(Expr* start) {
    if(start == NULL) {
        return ;
    }
    Expr* tmp;
    while(start != NULL) {
        tmp = start;
        start = start-> next;
        free(tmp);
    }
}

int checkArith(Expr* e) {
    if(e->exprType == constnum_e || e->exprType == arithexpr_e || 
        e->exprType == var_e || e->exprType == tableitem_e) {
        return 1;
    }
    return 0;
}

void restoreformalArgs(MinasTirithTouSpitiouMou* m) {
    assert(m != NULL);
    formalArgOffset = m->formalArgOffset;
}

void restoreLocalVars(MinasTirithTouSpitiouMou* m) {
    assert(m != NULL);
    functionLocalOffset = m->localVarOffset;
}

void insertOffsetStack(MinasTirithTouSpitiouMou* m, char* name, int activeLoops) {
    MinasTirithTouSpitiouMou* newnode = (MinasTirithTouSpitiouMou*) malloc(sizeof(MinasTirithTouSpitiouMou));
    newnode -> localVarOffset = functionLocalOffset;
    newnode -> formalArgOffset = formalArgOffset;
    newnode -> jumpQuad = getcurrQuad();
    newnode -> activeLoops = activeLoops;
    newnode -> name = name;
    newnode -> next = NULL;
    if(m->next == NULL) {
        m->next = newnode;
        return;
    }
        newnode->next = m->next;
        m->next = newnode;
        return;
}

MinasTirithTouSpitiouMou* popoffsetStack(MinasTirithTouSpitiouMou* m) {
    MinasTirithTouSpitiouMou* traverse = (MinasTirithTouSpitiouMou*)malloc(sizeof(MinasTirithTouSpitiouMou*));
    traverse = m -> next;
    m-> next = traverse-> next;
    traverse-> next = NULL;
    return traverse;
    
}
int getScopeSpaceCounter(void) {
    return scopeSpaceCounter;
}

void patchLabel(unsigned int quadNo, unsigned int label) {
    assert(quadNo < getcurrQuad());
    assert(!quads[quadNo].label);
    quads[quadNo].label = label;
}

Expr* make_call(Expr* lvalue, Expr* e_list, int scope, int line) {
    Expr* tmp, *result;
    Expr* func = (Expr*)malloc(sizeof(Expr));
    func = emit_ifTableItem(lvalue, scope, line);
    emitReverse(e_list, line);
    emit(call, NULL, NULL, func, getcurrQuad() + 1, line);
    result = (Expr*)malloc(sizeof(Expr));
    result = newExpr(var_e);
    result -> symbol = newTemp(scope, line);
    emit(getretval, NULL, NULL, result, getcurrQuad() + 1, line);

    return result;
}


void emitReverse(Expr* head, int line) {
    if(head == NULL) {
        return;
    }

    emitReverse(head -> next, line);


    emit(param, NULL, NULL, head, getcurrQuad() + 1, line);

}


void makeStatement(specialKeywords* s){
    s->breaklist = 0;
    s->contlist = 0;
    s->retlist = 0;
}

int newList(int i){
    quads[i].label = 0;
    return i;
}

int mergeList(int l1, int l2){
    if(l1 == 0){
        return l2;
    }
    else if(l2 == 0){
        return l1;
    }
    else{
        int i = l1;
        while(quads[i].label != 0){
            i = quads[i].label;
        }
        quads[i].label = l2;
        return l1;
    }
}

void patchList(loopStmt* head, int label, int loopScope){
    loopStmt* index, *tmp;
    tmp = (loopStmt*)malloc(sizeof(loopStmt));
    index = head;
    while(index != NULL) {
        printf("quadNo, label %d, %d\n", index -> quadNo, index -> loopScope);
        if(index -> loopScope == loopScope) {
            patchLabel(index -> quadNo, label);
        }
        index = index -> next;
    }
    return;
}

void insertLoopStmt(int quadNo, int loopScope, loopStmt* head) {
    loopStmt* newNode = (loopStmt *)malloc(sizeof(loopStmt));
    
    newNode -> quadNo = quadNo;
    newNode -> loopScope = loopScope;
    newNode -> next = NULL;

    if(head == NULL) {
        head = newNode;
        return;
    }

    newNode -> next = head -> next;
    head -> next = newNode;
    return;
}

loopStmt* popLoopStmt(loopStmt* head, int loopFlagNo) {
    loopStmt* out = (loopStmt*) malloc(sizeof(loopStmt));
    
    if(head == NULL) {
        return NULL;
    }

    if(head -> loopScope != loopFlagNo) {
        return NULL;
    }

    out = head -> next;
    head -> next = out-> next;
    out -> next = NULL;

    return out;
}
