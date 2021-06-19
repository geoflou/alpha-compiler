#include "SymbolTable.h"

enum iopcode{
    add,
    sub,
    mul,
    divide,
    mod,
    assign,
    nop,
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
    struct bool_stmt* trueList;
    struct bool_stmt* falseList;
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
    int label;
    unsigned line;
} quad;

typedef struct offsetStack{
    char* name;
    unsigned int jumpQuad;
    unsigned int activeLoops;
    unsigned int localVarOffset;
    unsigned int formalArgOffset;
    struct offsetStack* next; 
}MinasTirithTouSpitiouMou;

typedef struct callStruct{
    Expr* e_list;
    unsigned char method;
    char* name;
} callStruct;

typedef struct loopStruct {
    int test;
    int enter;
} loopStruct;

typedef struct special_stmt {
    int specialScope;
    int quadNo;
    struct special_stmt * next;
} specialStmt;

typedef struct bool_stmt {
    int quadNo;
    struct bool_stmt* next;
} boolStmt;

typedef struct temp_quad {
    enum iopcode op;
    Expr* arg1;
    Expr* arg2;
    unsigned label;
    int line;
    struct temp_quad* next;
} tempQuad;

#define EXPAND_SIZE 1024
#define CURR_SIZE (total * sizeof(quad))
#define NEW_SIZE (EXPAND_SIZE * sizeof(quad) + CURR_SIZE)

void insertID(char* name, int scope, int line);

void insertLocalID(char* name, int scope, int line);

void insertFormal(char* name, int scope, int line);

void expand(void);

void emit(enum iopcode op, Expr* arg1, Expr* arg2, Expr* result, int label, unsigned line);

unsigned currScopeOffset(void);

void incScopeOffset(void);

void resetScopeOffset(void);

void enterScopeSpace(void);

void exitScopeSpace(void);

Expr* lvalue_expr(SymbolTableEntry* sym, int scope, int line);

SymbolTableEntry* newTemp(int scope, int line);

Expr* newExpr(enum expr_t type);

Expr* newExpr_conststring(char* s);

Expr* newExpr_constbool(unsigned char b);

Expr* newExpr_constnum(double s);

void printQuads(void);

int isJumpLabel(quad* q);

char* getOpCode(quad* q);

char* getExpr(Expr* e);

unsigned int getcurrQuad(void);

Expr * emit_ifTableItem(Expr* e, int scope, int line);

Expr * member_item(Expr* e, char * name, int scope, int line); 

void deleteExprList(Expr* start);

int checkArith(Expr* e);

void restoreformalArgs(MinasTirithTouSpitiouMou* m);

void restoreLocalVars(MinasTirithTouSpitiouMou* m);

void insertOffsetStack(MinasTirithTouSpitiouMou* m, char* name, int activeLoops);

MinasTirithTouSpitiouMou* popoffsetStack(MinasTirithTouSpitiouMou* m);

int getScopeSpaceCounter(void);

void patchLabel(unsigned int quadNo, unsigned int label);

Expr* make_call(Expr* lvalue, Expr* e_list, int scope, int line);

void emitReverse(Expr* head, int line);

void patchList(specialStmt* list, int label, int specialScope);

void insertSpecialStmt(int quadNo, int specialScope, specialStmt* head);

specialStmt* popSpecialStmt(specialStmt* head, int flag);

void popSpecialScope(specialStmt* head, int flag);

boolStmt* insertBoolStmt(int quadNo, boolStmt* head);

void patchBoolList(int label, boolStmt* head);

void emptyBoolList(boolStmt* head);

int isEmptyBoolList(boolStmt* head);

boolStmt* mergeList(boolStmt* l1, boolStmt* l2);

void insertTempQuad(enum iopcode op, Expr* arg1, Expr* arg2, unsigned label, int line, tempQuad* head);

void emptyBuffer(tempQuad* head);

int isEmptyBuffer(tempQuad* head);

void emitTempQuads(tempQuad* head);

scopespace_t currScopeSpace(void);

unsigned getTotal(void);

quad* getQuads(void);
