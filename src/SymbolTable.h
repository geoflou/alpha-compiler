#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#define SYMBOL_TABLE_BUCKETS 1034
#define NON_SCOPE_BUCKETS 1024
#define SCOPE_BUCKETS 10
#define HASH_NUMBER 1787

enum SymbolType{
    GLOBAL,
    LOCAL,
    FORMAL,
    USERFUNC,
    LIBFUNC
};

typedef struct Variable{
    char *name;
    unsigned int scope;
    unsigned int line;
} Variable;


typedef struct Function{
    char *name;
    char ** arguments;
    unsigned int scope;
    unsigned int line;
    unsigned int label;
    unsigned int totalLocalVars;
} Function;

typedef struct SymbolTableEntry{
    int isActive;
    Variable *varVal;
    Function *funcVal;
    enum SymbolType type;
    int offset;
    struct SymbolTableEntry *next;
} SymbolTableEntry;

typedef struct ScopeNode {
    int label;
    struct SymbolTableEntry* list;
} ScopeNode;

#define EXPAND_SCOPE_SIZE 10
#define CURRENT_SCOPES (totalScopes * sizeof(ScopeNode))
#define NEW_SCOPE_SIZE (EXPAND_SCOPE_SIZE *sizeof(ScopeNode) * CURRENT_SCOPES)

void initTable(void);

void insertLibraryFunctions();

void insertFunction(char* name);

int hashForBucket(char *symbolName);

int hashForScope(int symbolScope);

void insertEntry(SymbolTableEntry *symbol);

SymbolTableEntry *lookupEverything(char *name, int scope);

SymbolTableEntry *lookupScope(char *name, int scope);

void hideEntries(int scope);

void hideFromScopeLink(int scope);

void hideFromBuckets(int scope);

void printEntries(void);

void printScope(SymbolTableEntry* scopeHead);

char *getEntryType(SymbolTableEntry *symbol);

char *getEntryName(SymbolTableEntry *symbol);

int getEntryLine(SymbolTableEntry *symbol);

int getEntryScope(SymbolTableEntry *symbol);

int comparelibfunc(char *name);

SymbolTableEntry *lookupforCalls(char *name, int scope);

SymbolTableEntry* updateEntry(char* name, int totals, int scope);
