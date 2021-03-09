#include "SymbolTable.h"

int i;

SymbolTableEntry *SymbolTable[1034];


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

    insertLibraryFunctions();

    return;
}


int hashForBucket(char *symbolName){
    assert(symbolName != NULL);
    return (atoi(symbolName) * HASH_NUMBER) % NON_SCOPE_BUCKETS;
}


int hashForScope(int symbolScope){
    return (symbolScope % SCOPE_BUCKETS) + NON_SCOPE_BUCKETS;
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
    scopeLinkSymbol -> next = NULL;
    scopeLinkSymbol -> type = symbol -> type;

    bucket = hashForBucket(name);
    scopeLink = hashForScope(scope);

    assert(SymbolTable[bucket] != NULL);
    assert(SymbolTable[scopeLink] != NULL);

    if(SymbolTable[bucket] -> next == NULL){
        SymbolTable[bucket] -> next = symbol;
    }    
    else{
        symbolIndex = SymbolTable[scopeLink] -> next;
        symbol -> next = SymbolTable[bucket] -> next;
        SymbolTable[bucket] -> next = symbol;
    }

    if(SymbolTable[scopeLink] -> next == NULL){
        SymbolTable[scopeLink] -> next = scopeLinkSymbol;
    }
    else{
        symbolIndex = SymbolTable[scopeLink] -> next;
        scopeLinkSymbol -> next = SymbolTable[scopeLink] -> next;
        SymbolTable[scopeLink] -> next = scopeLinkSymbol;
    }

    return;
}


SymbolTableEntry *lookupEverything(char *name){
    int bucket;
    SymbolTableEntry *symbolIndex;
    Variable *varTMP;
    Function *funcTMP;

    assert(name != NULL);
    bucket = hashForBucket(name);
    if(SymbolTable[bucket] == NULL){
        return NULL;
    }

    symbolIndex = SymbolTable[bucket];

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


SymbolTableEntry *lookupScope(char *name, int scope){
    int bucket;
    SymbolTableEntry *symbolIndex;
    Variable *varTMP;
    Function *funcTMP;

    assert(name != NULL);

    bucket = hashForScope(scope);
    if(SymbolTable[bucket] == NULL){
        return NULL;
    }

    symbolIndex = SymbolTable[bucket] -> next;

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

    bucket = hashForScope(scope);
    if(SymbolTable[bucket] == NULL){
        return;
    }

    symbolIndex = SymbolTable[bucket] -> next;

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
    Variable *varTMP;
    Function *funcTMP; 

    for(i = 0;i < 10;i++){
        
        printf("---------------  Scope #%d  ---------------\n", i);
        symbolIndex = SymbolTable[NON_SCOPE_BUCKETS + i];

        if(symbolIndex == NULL){
            continue;
        }
        symbolIndex = symbolIndex -> next;
        while(symbolIndex != NULL){
            printf("\"%s\"  [%s]    (line %d)   (scope %d)\n",getEntryName(symbolIndex),
                getEntryType(symbolIndex), getEntryLine(symbolIndex), getEntryScope(symbolIndex));
            symbolIndex = symbolIndex -> next;
        }

    }
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
            printf("Error, cannot use library function as name variable\n");
            return -1;
        }

        i=strcmp(name,input);
        if(i==0){
            printf("Error, cannot use library function as name variable\n");
            return -1;
        }
        
        i=strcmp(name,objectmemberkeys);
        if(i==0){
            printf("Error, cannot use library function as name variable\n");
            return -1;
        }

        i=strcmp(name,objecttotalmembers);
        if(i==0){
            printf("Error, cannot use library function as name variable\n");
            return -1;
        }

        i=strcmp(name,objectcopy);
        if(i==0){
            printf("Error, cannot use library function as name variable\n");
            return -1;
        }

        i=strcmp(name,totalarguments);
        if(i==0){
            printf("Error, cannot use library function as name variable\n");
            return -1;
        }

        i=strcmp(name,argument);
        if(i==0){
            printf("Error, cannot use library function as name variable\n");
            return -1;
        }

        i=strcmp(name,typeof1);
        if(i==0){
            printf("Error, cannot use library function as name variable\n");
            return -1;
        }

        i=strcmp(name,strtonum);
        if(i==0){
            printf("Error, cannot use library function as name variable\n");
            return -1;
        }

        i=strcmp(name,sqrt);
        if(i==0){
            printf("Error, cannot use library function as name variable\n");
            return -1;
        }

        i=strcmp(name,cos);
        if(i==0){
            printf("Error, cannot use library function as name variable\n");
            return -1;
        }

        i=strcmp(name,sin);
        if(i==0){
            printf("Error, cannot use library function as name variable\n");
            return -1;
        }

    return 0;
}