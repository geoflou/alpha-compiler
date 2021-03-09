#include "utilities.h"

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
        yyerror("A function has taken already that name!");
        return;
    }

    return;
}

void insertLocalID(char* name, int scope, int line) {
    SymbolTableEntry *temp;
    SymbolTableEntry *new_entry;
    Variable *new_var;

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
        yyerror("A user function with that name already exists!\n");
        return;
    }   

    if(comparelibfunc(name) == -1) {
        return;
    }

    return;
}