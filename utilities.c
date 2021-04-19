#include "utilities.h"

quad* quads = (quad *) 0;
unsigned total = 0;
unsigned currQuad = 0;

unsigned programVarOffset = 0;
unsigned functionLocalOffset = 0;
unsigned formalArgOffset = 0;
unsigned scopeSpaceCounter = 1; 

void insertID(char* name, int scope, int line) {
    SymbolTableEntry *temp;
    SymbolTableEntry *new_entry;
    Variable *new_var;
    
    temp = lookupEverything(name, scope);

    if(temp == NULL) {
        new_entry = (SymbolTableEntry*)malloc(sizeof(SymbolTableEntry));
        new_var = (Variable*)malloc(sizeof(Variable));

        new_var -> name = name;
        new_var -> scope = scope;
        new_var -> line = line;

        new_entry -> isActive = 1;
        new_entry -> varVal = new_var;
        new_entry -> funcVal = NULL;
        
        scope == 0? (new_entry -> type = GLOBAL): (new_entry -> type = LOCAL);

        insertEntry(new_entry);
        return;
    }
    if(getEntryType(temp) == "USERFUNC" && temp -> isActive == 1) {
        printf("\033[31mERROR: A function has taken already that name!\033[0m");
    }
    
    return;
}

void insertLocalID(char* name, int scope, int line) {
    SymbolTableEntry *temp;
    SymbolTableEntry *new_entry;
    Variable *new_var;

    if(comparelibfunc(name) == -1) {
        printf("\033[31mERROR: Function name redefinition \"%s\" is a library function\033[0m", name);
        yyerror("\t");
        return;
    }

    temp = lookupScope(name, scope);

    if(temp == NULL) {
        new_entry = (SymbolTableEntry*)malloc(sizeof(SymbolTableEntry));
        new_var = (Variable*)malloc(sizeof(Variable));

        new_var -> name = name;
        new_var -> scope = scope;
        new_var -> line = line;

        new_entry -> isActive = 1;
        new_entry -> varVal = new_var;
        new_entry -> funcVal = NULL;
        
        scope == 0? (new_entry -> type = GLOBAL): (new_entry -> type = LOCAL);

        insertEntry(new_entry);
        return;
    }

    if(temp -> type == USERFUNC && temp -> isActive == 1) {
        printf("\033[31mERROR: A user function with that name already exists!\033[0m\n");
    }   

    return;
}

void insertFormal(char* name, int scope, int line) {
    SymbolTableEntry *temp;
    SymbolTableEntry *new_entry;
    Variable *new_var;
    
    assert(name != NULL);
    if(comparelibfunc(name) == -1) {
         printf("\033[31mERROR: Function name redefinition \"%s\" is a library function\033[0m", name);
        yyerror("\t");
        return;
    }

    new_entry = (SymbolTableEntry*)malloc(sizeof(SymbolTableEntry));
    new_var = (Variable*)malloc(sizeof(Variable));

    new_var -> name = name;
    new_var -> scope = scope;
    new_var -> line = line;

    new_entry -> isActive = 1;
    new_entry -> varVal = new_var;
    new_entry -> funcVal = NULL;
    new_entry -> type = FORMAL;

    insertEntry(new_entry);
    return;
}

unsigned currScopeOffset(void) {
    if(scopeSpaceCounter == 1) {
        return programVarOffset;
    }

    if(scopeSpaceCounter % 2 == 0) {
        return formalArgOffset;
    }

    return functionLocalOffset;
}

void enterScopeSpace(void) {
    ++scopeSpaceCounter;
    return;
}

void exitScopeSpace(void){
    assert(scopeSpaceCounter > 1);
    --scopeSpaceCounter;
    return;
}


void expand(void) {
    assert(total == currQuad);
    quad* p = (quad*) malloc(NEW_SIZE);
    if(quads) {
        memcpy(p, quads, CURR_SIZE);
        free(quads);
    }

    quads = p;
    total += EXPAND_SIZE;

    return;
}

void emit(enum iopcode op, Expr* arg1, Expr* arg2, Expr* result, unsigned label, unsigned line) {
    if(currQuad == total) {
        expand();
    }

    quad* p = quads + currQuad++;
    p -> op = op;
    p -> arg1 = arg1;
    p -> arg2 = arg2;
    p -> result = result;
    p -> label = label;
    p -> line - line;

    return;
}

