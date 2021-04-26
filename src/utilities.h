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

enum expr_t {
    var_e,
    tableitem_e,

    programfunc_e,
    libraryfunc_e,

    arithexpr_e,
    boolexpr_e,
    assignexpr_e,
    newtable_e,

    constnum_e,
    constbool_e,
    conststring_e,
    nil_e,
};

typedef struct expr{
    enum expr_t exprType;
    SymbolTableEntry* symbol;
    struct expr* index;
    struct expr* indexedelem_value;
    double numConst;
    char* strConst;
    unsigned char boolConst;
    struct expr* next;
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

void incScopeOffset(void);

void resetScopeOffset(void);

void enterScopeSpace(void);

void resetScopeOffset(void);

void exitScopeSpace(void);

Expr* lvalue_expr(SymbolTableEntry* sym, int scope, int line);

SymbolTableEntry* newTemp(int scope, int line);

Expr* newExpr(enum expr_t type);

Expr* newExpr_conststring(char* s);

Expr* newExpr_constbool(unsigned char b);

Expr* newExpr_constnum(double s);

void printQuads(void);

char* getOpCode(quad* q);

char* getResult(Expr* e);

char* getArg(Expr* e);

unsigned int getcurrQuad(void);

Expr * emit_ifTableItem(Expr* e, int scope, int line);

Expr * member_item(Expr* e, char * name, int scope, int line); 

void deleteExprList(Expr* start);

int checkArith(Expr* e);