#include "SymbolTable.h"

enum iopcode{
    assign,
    add,
    sub,
    mul,
    divide,
    mod,
    uminus,
    and,
    or,
    not,
    if_eq,
    if_noteq,
    if_lesseq,
    if_greatereq,
    if_less,
    if_greater,
    call,
    param,
    ret,
    getretval,
    funcstart,
    funcend,
    tablecreate,
    tablegetelem,
    tablesetelem,
    jump
};

enum scopespace_t {
    programVar,
    functionLocal,
    formalArg
};

enum symbol_t {
    var_s,
    programFunc_s,
    libraryFunc_s
};

typedef struct expr{
    int x;
    int y;
} Expr;

typedef struct quad {
    enum iopcode op;
    Expr *arg1;
    Expr *arg2;
    Expr *result;
    unsigned label;
    unsigned line;
} quad;

#define EXPAND_SIZE 1024
#define CURR_SIZE (total * sizeof(quad))
#define NEW_SIZE (EXPAND_SIZE * sizeof(quad) + CURR_SIZE)

void insertID(char* name, int scope, int line);

void insertLocalID(char* name, int scope, int line);

void insertFormal(char* name, int scope, int line);

void expand(void);

void emit(enum iopcode op, Expr* arg1, Expr* arg2, Expr* result, unsigned label, unsigned line);

unsigned currScopeOffset(void);

void enterScopeSpace(void);

void exitScopeSpace(void);