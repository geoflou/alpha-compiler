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
        printf("\033[31mERROR: A function has taken already that name!\033[0m");
    }
    
    return;
}

void insertLocalID(char* name, int scope, int line) {
    SymbolTableEntry *temp;
    SymbolTableEntry *new_entry;
    Variable *new_var;

    if(comparelibfunc(name) == -1) {
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