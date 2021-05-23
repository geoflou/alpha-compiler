#include "SymbolTable.h"

int i;

SymbolTableEntry *SymbolTable[SYMBOL_TABLE_BUCKETS];

ScopeNode* scopes = NULL;
unsigned currentMaxScope = 0;
unsigned totalScopes = 0;


void initTable(void){
    int i;

    for(i = 0;i < SYMBOL_TABLE_BUCKETS;i++){
        SymbolTable[i] = (SymbolTableEntry*)malloc(sizeof(SymbolTableEntry));
        SymbolTable[i] -> isActive = 0;
        SymbolTable[i] -> next = NULL;
        SymbolTable[i] -> type = GLOBAL;
        SymbolTable[i] -> funcVal = NULL;
        SymbolTable[i] -> varVal = NULL;
    }

    expandScopes(5);
    insertLibraryFunctions();

    return;
}

int hashForBucket(char *symbolName){
    assert(symbolName != NULL);
    return (atoi(symbolName) * HASH_NUMBER) % NON_SCOPE_BUCKETS;
}


void insertEntry(SymbolTableEntry *symbol){
    int bucket, scopeLink;
    int scope;
    char *name;
    SymbolTableEntry *scopeLinkSymbol, *symbolIndex;

    assert(symbol != NULL);

    if(symbol -> funcVal != NULL){
        scope = symbol -> funcVal -> scope;
        name =  symbol -> funcVal -> name;
    }

    if(symbol -> varVal != NULL){
        scope = symbol -> varVal -> scope;
        name = symbol -> varVal -> name;
    }

    scopeLinkSymbol = (SymbolTableEntry *) malloc(sizeof(SymbolTableEntry));
    scopeLinkSymbol -> isActive = symbol ->isActive;
    scopeLinkSymbol -> varVal = symbol -> varVal;
    scopeLinkSymbol -> funcVal = symbol -> funcVal;
    scopeLinkSymbol -> offset = symbol -> offset;
    scopeLinkSymbol -> next = NULL;
    scopeLinkSymbol -> type = symbol -> type;

    bucket = hashForBucket(name);

    assert(SymbolTable[bucket] != NULL);

    if(SymbolTable[bucket] -> next == NULL){
        SymbolTable[bucket] -> next = symbol;
    }    
    else{
        symbol -> next = SymbolTable[bucket] -> next;
        SymbolTable[bucket] -> next = symbol;
    }

    insertInScope(scopeLinkSymbol);

    return;
}

SymbolTableEntry *lookupEverything(char *name, int scope){
    SymbolTableEntry *symbolIndex;
    int i = 0;

    assert(name != NULL);

    for(i = scope; i >= 0;i--) {
        symbolIndex = lookupScope(name, i);
        
        if(symbolIndex == NULL) {
            continue;
        }

        if(symbolIndex -> isActive == 1) {
            return  symbolIndex; 
        }

    }

    return NULL;
}

SymbolTableEntry *lookupforCalls(char *name, int scope){
    SymbolTableEntry *symbolIndex;
    int i = 0;

    assert(name != NULL);
    for(i = scope; i >= 0;i--) {
        symbolIndex = lookupScope(name, i);
        
        if(symbolIndex == NULL) {
            continue;
        }

        if(symbolIndex -> isActive == 1 && symbolIndex -> type == USERFUNC) {
            return  symbolIndex; 
        }else if (symbolIndex -> isActive == 1 && i == scope){
            return NULL;
        }

    }

    return NULL;
}

SymbolTableEntry *lookupScope(char *name, int scope){
    int bucket;
    SymbolTableEntry *symbolIndex;
    ScopeNode* scopeIndex;
    Variable *varTMP;
    Function *funcTMP;

    if(scope >= totalScopes - 1) {
        return NULL;
    }

    assert(name != NULL);

    scopeIndex = scopes;
    while(scopeIndex != NULL && scopeIndex -> label != scope) {
        scopeIndex = scopeIndex -> next;
    }

    symbolIndex = scopeIndex -> list -> next;
    if(symbolIndex == NULL) {
        return NULL;
    }

    while(symbolIndex != NULL){
        
        if(symbolIndex -> varVal != NULL){
            varTMP = symbolIndex -> varVal;
            if(strcmp(varTMP -> name, name) == 0){
                return symbolIndex;
            }
        }

        if(symbolIndex -> funcVal != NULL){
            funcTMP = symbolIndex -> funcVal;
            if(strcmp(funcTMP -> name, name) == 0){
                return symbolIndex;
            }
        }
        symbolIndex = symbolIndex -> next;
    }

    return NULL;
}

void hideEntries(int scope){
    hideFromScopeLink(scope);
    hideFromBuckets(scope);
    return;
}

void hideFromScopeLink(int scope){
    int bucket;
    SymbolTableEntry *symbolIndex;
    ScopeNode* scopeIndex = scopes;

    if(scope > currentMaxScope) {
        return;
    }

    while(scopeIndex != NULL && scopeIndex -> label != scope) {
        scopeIndex = scopeIndex -> next;
    }

    assert(scopeIndex != NULL);
    
    symbolIndex = scopeIndex -> list -> next;
    if(symbolIndex == NULL){
        return;
    }


    while(symbolIndex != NULL){
        symbolIndex -> isActive = 0;
        symbolIndex = symbolIndex ->next;
    }

    return;
}

void hideFromBuckets(int scope){
    int i;
    SymbolTableEntry *symbolIndex;
    Variable *varTMP;
    Function *funcTMP;
    
    for(i = 0;i < NON_SCOPE_BUCKETS;i++){
        
        if(SymbolTable[i] == NULL){
            continue;
        }

        symbolIndex = SymbolTable[i] -> next;

        while(symbolIndex != NULL){

            if(symbolIndex -> varVal != NULL){
                varTMP = symbolIndex -> varVal;
                if(varTMP -> scope == scope){
                    symbolIndex -> isActive = 0;
                }
            }

            if(symbolIndex -> funcVal != NULL){
                funcTMP = symbolIndex -> funcVal;
                if(funcTMP -> scope == scope){
                    symbolIndex -> isActive = 0;
                }
            }

            symbolIndex = symbolIndex ->next;
        }
    }

    return;
}

void printEntries(void){
    int i;
    SymbolTableEntry *symbolIndex;
    ScopeNode* scopeIndex = scopes -> next;
    Variable *varTMP;
    Function *funcTMP; 

    for(i = 0;i <= currentMaxScope;i++){

        printf("---------------  Scope #%d  ---------------\n", i);
        symbolIndex = scopeIndex -> list -> next;

        if(symbolIndex == NULL){
            scopeIndex = scopeIndex -> next;
            continue;
        }

        printScope(symbolIndex);
        scopeIndex = scopeIndex -> next;
    }
    printf("-------------------------------------------\n");
    return;
}

void printScope(SymbolTableEntry* scopeHead) {
    if(scopeHead == NULL) {
        return;
    }

    printScope(scopeHead -> next);

    printf("\"%s\"  [%s]    (line %d)   (scope %d)\n",getEntryName(scopeHead),
                getEntryType(scopeHead), getEntryLine(scopeHead), getEntryScope(scopeHead));

    return;
}

char *getEntryType(SymbolTableEntry *symbol){
    switch (symbol -> type)
    {
    case GLOBAL:
        return "global variable";
    
    case LOCAL:
        return "local variable";
    
    case FORMAL:
        return "formal argument";

    case USERFUNC:
        return "user function";

    case LIBFUNC:
        return "library function";
    
    default:
        assert(0);
    }
}

char *getEntryName(SymbolTableEntry *symbol){
    Variable *varTMP;
    Function *funcTMP;

    if(symbol -> funcVal != NULL){
        funcTMP = symbol -> funcVal;
        return funcTMP -> name;
    }

    if(symbol -> varVal != NULL){
        varTMP = symbol -> varVal;
        return varTMP -> name;
    }

    assert(0);
}

int getEntryLine(SymbolTableEntry *symbol){
    Variable *varTMP;
    Function *funcTMP;

    if(symbol -> funcVal != NULL){
        funcTMP = symbol -> funcVal;
        return funcTMP -> line;
    }

    if(symbol -> varVal != NULL){
        varTMP = symbol -> varVal;
        return varTMP -> line;
    }

    assert(0);
}

int getEntryScope(SymbolTableEntry *symbol){
    Variable *varTMP;
    Function *funcTMP;

    if(symbol -> funcVal != NULL){
        funcTMP = symbol -> funcVal;
        return funcTMP -> scope;
    }

    if(symbol -> varVal != NULL){
        varTMP = symbol -> varVal;
        return varTMP -> scope;
    }

    assert(0);
}

void insertLibraryFunctions(void){
    insertFunction("print");
    insertFunction("input");
    insertFunction("objectmemberkeys");
    insertFunction("objectcopy");
    insertFunction("totalarguments");
    insertFunction("argument");
	insertFunction("typeof");
	insertFunction("strtonum");
	insertFunction("sqrt");
	insertFunction("cos");
	insertFunction("sin");

    return;
}

void insertFunction(char* name) {
    SymbolTableEntry *entry = (SymbolTableEntry*)malloc(sizeof(SymbolTableEntry));
    Function *func = (Function *)malloc(sizeof(Function));
    entry -> isActive = 1;
    func -> name = name;
    func -> scope = 0;
    func -> line = 0;
    entry -> funcVal = func;
    entry -> type = LIBFUNC;
    entry -> next = NULL;
    insertEntry(entry);
}

int comparelibfunc(char *name){
        
    char *print = malloc(sizeof(char*)*10);
    char *input = malloc(sizeof(char*)*10);
    char *objectmemberkeys = malloc(sizeof(char*)*10);
    char *objecttotalmembers = malloc(sizeof(char*)*10);
    char *objectcopy = malloc(sizeof(char*)*10);
    char *totalarguments = malloc(sizeof(char*)*10);
    char *argument = malloc(sizeof(char*)*10);
    char *typeof1 = malloc(sizeof(char*)*10);
    char *strtonum = malloc(sizeof(char*)*10);
    char *sqrt = malloc(sizeof(char*)*10);
    char *cos = malloc(sizeof(char*)*10);
    char *sin = malloc(sizeof(char*)*10);

    print = "print";
    input = "input";
    objectmemberkeys = "objectmemberkeys";
    objecttotalmembers = "objecttotalmembers";
    objectcopy = "objectcopy";
    totalarguments = "totalarguments";
    argument = "argument";
    typeof1 = "typeof";
    strtonum = "strtonum";
    sqrt = "sqrt";
    cos = "cos";
    sin = "sin";

        i=strcmp(name,print);
        if(i==0){
            return -1;
        }

        i=strcmp(name,input);
        if(i==0){
            return -1;
        }
        
        i=strcmp(name,objectmemberkeys);
        if(i==0){
            return -1;
        }

        i=strcmp(name,objecttotalmembers);
        if(i==0){
            return -1;
        }

        i=strcmp(name,objectcopy);
        if(i==0){
            return -1;
        }

        i=strcmp(name,totalarguments);
        if(i==0){
            return -1;
        }

        i=strcmp(name,argument);
        if(i==0){
            return -1;
        }

        i=strcmp(name,typeof1);
        if(i==0){
            return -1;
        }

        i=strcmp(name,strtonum);
        if(i==0){
            return -1;
        }

        i=strcmp(name,sqrt);
        if(i==0){
            return -1;
        }

        i=strcmp(name,cos);
        if(i==0){
            return -1;
        }

        i=strcmp(name,sin);
        if(i==0){
            return -1;
        }

    return 0;
}

SymbolTableEntry* updateEntry(char* name, int totals, int scope) {
    SymbolTableEntry* s;
    s = lookupScope(name, scope);
    assert(s !=NULL);
    s->funcVal->totalLocalVars = totals;
    return s;
}

void expandScopes(int maxScope) {
    int i = 0;
    while(i < maxScope) {
        createScope();
        i++;
    }
    return;
}

void createScope(void) {
    ScopeNode* index = scopes;
    ScopeNode* newScope = (ScopeNode*) malloc(sizeof(ScopeNode));
    
    newScope -> next = NULL;
    newScope -> list = (SymbolTableEntry*) malloc(sizeof(SymbolTableEntry));
    newScope -> label = totalScopes;
    totalScopes++;

    if(scopes == NULL) {
        scopes = (ScopeNode *) malloc(sizeof(ScopeNode));
        scopes -> label = -1;
        scopes -> list = NULL;
        scopes -> next = newScope;
        return;
    }

    index = scopes;
    while(index -> next != NULL) {
        index = index -> next;
    }

    index -> next = newScope;

    return;
}

void insertInScope(SymbolTableEntry * entry) {
    assert(entry != NULL);
    int scope = getEntryScope(entry);
    ScopeNode* scopeIndex = scopes;
    SymbolTableEntry* tmp;

    if(scope >= totalScopes - 1) {
        expandScopes(scope);
    }
    assert(scopeIndex != NULL);
    while(scopeIndex != NULL && scopeIndex -> label != scope) {
        scopeIndex = scopeIndex -> next;
    }

    if(scope > currentMaxScope) {
        currentMaxScope = scope;
    }

    tmp = scopeIndex -> list;
    if(tmp -> next == NULL) {
        tmp -> next = entry;
        return;
    }

    entry -> next = tmp -> next;
    tmp -> next = entry;

    return;

}

