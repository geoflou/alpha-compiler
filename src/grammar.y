%{
    #include <stdlib.h>
    #include <stdio.h>
    #include "utilities.h"
    #include "grammar.h"

    int yyerror(char* message);
    int yylex(void);

    extern int yylineno;
    extern char* yytext;
    extern FILE* yyin;

    int scope = 0;

    int retquad = 0;

    char* currFunc;

    Expr* exprStack = (Expr *) 0;

    specialStmt* breakStack = (specialStmt *) 0;
    specialStmt* continueStack = (specialStmt *) 0;
    specialStmt* retStack = (specialStmt *) 0;

    MinasTirithTouSpitiouMou* offsetStack = (MinasTirithTouSpitiouMou*) 0;

    Function* temp_func;
    Expr* lushAlex = (Expr *) 0;
    int arg_index = 0;

    int anonFuncCounter = 0;
    int loopFlag = 0;
    int funcFlag = 0;
    int memberflag = 0;

%}

%union{
    int intVal;
    char *strVal;
    double doubleVal;
    struct expr *exp;
    struct callStruct *calls;
    struct loopStruct *loops;
}


%start program

%expect 1

%token <strVal> ID
%token <intVal> INTEGER
%token <doubleVal> REAL
%token <strVal> STRING

%type <exp> const
%type <exp> lvalue
%type <exp> expr
%type <exp> assignexpr
%type <exp> primary
%type <exp> member
%type <exp> objectdef
%type <exp> indexed
%type <exp> indexedelem
%type <exp> term
%type <exp> funcdef
%type <exp> call

%type <calls> normcall
%type <calls> methodcall
%type <calls> callsuffix

%type <intVal> ifprefix
%type <intVal> elseprefix
%type <intVal> whilecond
%type <intVal> whilestart

%type <intVal> M
%type <intVal> N

%type <loops> forprefix


%token IF
%token ELSE
%token WHILE


%token FOR
%token <strVal> FUNCTION
%token RETURN
%token BREAK
%token CONTINUE

%token LOCAL_KEYWORD
%token TRUE
%token FALSE
%token NIL
%token WHITESPACE


%right OPERATOR_ASSIGN
%left OR
%left AND
%nonassoc OPERATOR_EQ OPERATOR_NEQ
%nonassoc OPERATOR_GRT OPERATOR_LES OPERATOR_GRE OPERATOR_LEE
%left OPERATOR_PLUS OPERATOR_MINUS
%left OPERATOR_MUL OPERATOR_DIV OPERATOR_MOD
%right NOT OPERATOR_PP OPERATOR_MM UMINUS

%left SEMICOLON COLON COMMA DOUBLE_COLON
%left LEFT_BRACE RIGHT_BRACE
%left DOT DOUBLE_DOT    
%left LEFT_PARENTHESIS RIGHT_PARENTHESIS
%left LEFT_BRACKET RIGHT_BRACKET


%%
program: set   {printf("set -> program\n");}
    |   {printf("EMPTY -> program\n");}
    ;

set: stmt
    |set stmt {printf("set stmt -> set\n");}
    ;

breakstmt: BREAK SEMICOLON{
    printf("break; -> break\n");
    if(loopFlag == 0) {
        yyerror("\033[31mERROR: break statement outside loop\033[0m\t");
        return 1;
    }
    insertSpecialStmt(getcurrQuad(), loopFlag, breakStack);
    emit(jump, NULL, NULL, NULL, 0, yylineno);
}
;

continuestmt: CONTINUE SEMICOLON{
    printf("continue; -> continue\n");
    if(loopFlag == 0) {
        yyerror("\033[31mERROR: continue statement outside loop\033[0m\t");
        return 1;
    }
    
    insertSpecialStmt(getcurrQuad(), loopFlag, continueStack);
    emit(jump,NULL,NULL,NULL,0,yylineno);
}
;
stmt: expr SEMICOLON    {
        printf("expr ; -> stmt\n");
        if(!isEmptyBoolList($1 -> trueList) || !isEmptyBoolList($1 -> falseList)) {
            patchBoolList(getcurrQuad(), $1 -> trueList);
            patchBoolList(getcurrQuad() + 2, $1 -> falseList);

            emit(assign, newExpr_constbool(1), NULL, $1, getcurrQuad() + 1, yylineno);
            emit(jump, NULL, NULL, NULL, getcurrQuad() + 2, yylineno);
            emit(assign, newExpr_constbool(0), NULL, $1, getcurrQuad() + 1, yylineno);
            
            emptyBoolList($1 -> trueList);
            emptyBoolList($1 -> falseList);
            
        }

    }
    |ifstmt {printf("ifstmt -> stmt\n");}
    |whilestmt  {
        printf("whilestmt -> stmt\n"); 
    }
    |forstmt    {
        printf("forstmt -> stmt\n");
    }
    |returnstmt    {
                printf("returnstmt -> stmt\n");
                if(funcFlag == 0) {
                    yyerror("\033[31mERROR: return statement outside function\033[0m\t");
                    return 1;
                }
            }
    |breakstmt    {
            printf("break; -> stmt\n");
        }
    |continuestmt {
            printf("continue; -> stmt\n");
        }
    |block  {printf("block -> stmt\n");}
    |funcdef    {printf("funcdef -> stmt\n");}
    |SEMICOLON  {printf("; -> stmt\n");}
    ;

term: LEFT_PARENTHESIS expr RIGHT_PARENTHESIS   {
        printf("(expr) -> term\n");
        $$ = $2;
    }
    |OPERATOR_MINUS expr  %prec UMINUS{
        printf("- expr -> term\n");
        if(checkArith($2) == 1){
            $$ = newExpr(arithexpr_e);
            $$ -> symbol = newTemp(scope, yylineno);
            emit(uminus, $2, NULL, $$, getcurrQuad()+1, yylineno);
        }else {
            printf("\033[31mERROR: unary minus expression used outside arithmetic expression \033[0m \n");
            return 1;
        }
    } 
    |NOT expr  		{
            printf("not expr -> term\n");
            boolStmt* tmp ;
            $$ = newExpr(boolexpr_e);
            $$ -> symbol = newTemp(scope, yylineno);
            if($2 -> exprType != boolexpr_e) {

                $2 -> trueList = insertBoolStmt(getcurrQuad(), $2 -> trueList);
                emit(if_eq, newExpr_constbool(1), NULL, $2, 0, yylineno);

                $2 -> falseList = insertBoolStmt(getcurrQuad(), $2 -> falseList);
                emit(jump, NULL, NULL, NULL, 0, yylineno);


            }
            //SEGFAULT FOR DEBUG
            $$ -> trueList = $2 -> falseList;
            $$ -> falseList = $2 -> trueList;
            
            if($2 -> exprType != constbool_e) {
                patchBoolList(getcurrQuad(), $$ -> trueList);
                patchBoolList(getcurrQuad() + 2, $$ -> falseList);
                emptyBoolList($$ -> trueList);
                emptyBoolList($$ -> falseList);
                $$ -> trueList = NULL;
                $$ -> falseList = NULL;
            }

            //TODO: when should i print these 3 -->
            emit(assign, newExpr_constbool(1), NULL, $$, getcurrQuad() + 1, yylineno);
            emit(jump, NULL, NULL, NULL, getcurrQuad() + 2, yylineno);
            emit(assign, newExpr_constbool(0), NULL, $$, getcurrQuad() + 1, yylineno);
        }
    |OPERATOR_PP lvalue {
            printf("++lvalue -> term\n");
            SymbolTableEntry* temp;
            if(memberflag == 0) {
                temp = lookupScope(yylval.strVal, scope);
            } else {
                temp = lookupScope(getEntryName($2 -> symbol), scope);
            }

            if(temp != NULL) {
                if(temp -> type == USERFUNC || temp -> type == LIBFUNC) {
                    yyerror("\033[31mERROR: Function cannot be used as lvalue\033[0m\t");
                    return 1;
                }
            }
            if(checkArith($2) == 1) {
                if($2 -> exprType == tableitem_e) {
                    $$ = emit_ifTableItem($2, scope, yylineno);
                    emit(add, $$, newExpr_constnum(1), $$, getcurrQuad() + 1, yylineno);
                    emit(tablesetelem, $2, $2 -> index, $$, getcurrQuad() + 1, yylineno);
                } else {
                    emit(add, $2, newExpr_constnum(1), $2, getcurrQuad() + 1, yylineno);
                    $$ = newExpr(arithexpr_e);
                    $$ -> symbol = newTemp(scope, yylineno);
                    emit(assign, $2, NULL, $$, getcurrQuad() + 1, yylineno);
                }
            }
            else {
                printf("\033[31mERROR: plus plus expression used outside arithmetic expression \033[0m \n");
                return 1;
            }
        }
    |lvalue {
                SymbolTableEntry* temp;
                if(memberflag == 0) {
                    temp = lookupScope(yylval.strVal, scope);
                } else {
                    temp = lookupScope(getEntryName($1 -> symbol), scope);
                }
                if(temp != NULL) {
                    if(temp -> type == USERFUNC || temp -> type == LIBFUNC) {
                        yyerror("\033[31mERROR: Function cannot be used as lvalue\033[0m\t");
                        return 1;
                    }
                }
    }   OPERATOR_PP {
        printf("lvalue++ -> term\n");
            if(checkArith($1) == 1){
                $$ = newExpr(var_e);
                $$ -> symbol = newTemp(scope, yylineno);
                if($1-> exprType == tableitem_e){
                    Expr* n = (Expr*)malloc(sizeof(Expr)); 
                    n = emit_ifTableItem($1, scope, yylineno);
                    emit(assign, n, NULL, $$, getcurrQuad()+1, yylineno);
                    emit(add, n, newExpr_constnum(1), n, getcurrQuad()+1, yylineno);
                    emit(tablesetelem, $1, $1-> index, n, getcurrQuad()+1, yylineno);
                } else{
                    emit(assign, $1, NULL, $$, getcurrQuad()+1, yylineno);
                    emit(add, $1, newExpr_constnum(1), $1, getcurrQuad()+1, yylineno);
                }
            }
            else {
            printf("\033[31mERROR: expression plus plus used outside arithmetic expression \033[0m \n");
            return 1;
        }
        }
    |OPERATOR_MM lvalue {
            printf("--lvalue -> term\n");
            SymbolTableEntry* temp;
            if(memberflag == 0) {
                temp = lookupScope(yylval.strVal, scope);
            } else {
                temp = lookupScope(getEntryName($2 -> symbol), scope);
            }
            if(temp != NULL) {
                if(temp -> type == USERFUNC || temp -> type == LIBFUNC) {
                    yyerror("\033[31mERROR: Function cannot be used as lvalue\033[0m\t");
                    return 1;
                }
            }
            if(checkArith($2) == 1) {
                if($2 -> exprType == tableitem_e) {
                    $$ = emit_ifTableItem($2, scope, yylineno);
                    emit(sub, $$, newExpr_constnum(1), $$, getcurrQuad() + 1, yylineno);
                    emit(tablesetelem, $2, $2 -> index, $$, getcurrQuad() + 1, yylineno);
                } else {
                    emit(sub, $2, newExpr_constnum(1), $2, getcurrQuad() + 1, yylineno);
                    $$ = newExpr(arithexpr_e);
                    $$ -> symbol = newTemp(scope, yylineno);
                    emit(assign, $2, NULL, $$, getcurrQuad() + 1, yylineno);
                }
            }
            else {
            printf("\033[31mERROR: minus minus expression used outside arithmetic expression \033[0m \n");
            return 1;
        }
        }
    |lvalue {
            SymbolTableEntry* temp;
            if(memberflag == 0) {
                temp = lookupScope(yylval.strVal, scope);
            } else {
                temp = lookupScope(getEntryName($1 -> symbol), scope);
            }
            if(temp != NULL) {
                if(temp -> type == USERFUNC || temp -> type == LIBFUNC) {
                    yyerror("\033[31mERROR: Function cannot be used as lvalue\033[0m\t");
                    return 1;
                }
            }
        } OPERATOR_MM {
            printf("lvalue-- -> term\n");
            if(checkArith($1) == 1){
                $$ = newExpr(var_e);
                $$ -> symbol = newTemp(scope, yylineno);
                if($1 -> exprType == tableitem_e) {
                    Expr* n = (Expr*)malloc(sizeof(Expr)); 
                    n = emit_ifTableItem($1, scope, yylineno);
                    emit(assign, n, NULL, $$, getcurrQuad()+1, yylineno);
                    emit(sub, n, newExpr_constnum(1), n, getcurrQuad()+1, yylineno);
                    emit(tablesetelem, $1, $1-> index, n, getcurrQuad()+1, yylineno);
                } else{
                    emit(assign, $1, NULL, $$, getcurrQuad() + 1, yylineno);
                    emit(sub, $1, newExpr_constnum(1), $1, getcurrQuad() + 1, yylineno);
                }
            }
            else {
            printf("\033[31mERROR: expression minus minus used outside arithmetic expression \033[0m \n");
            return 1;
        }

        }
    |primary    {
        printf("primary -> term\n");
        $$ = $1;
        }
    ;

expr: assignexpr    { printf("assignexpr -> expr\n");}
    | expr OPERATOR_PLUS expr   {
        printf("expr + expr -> expr\n");
        $$ = newExpr(arithexpr_e);
        $$ -> symbol = newTemp(scope, yylineno);
        emit(add, $1, $3, $$, getcurrQuad() + 1, yylineno);
    }
    | expr OPERATOR_MINUS expr  {
        printf("expr - expr -> expr\n");
        $$ = newExpr(arithexpr_e);
        $$ -> symbol = newTemp(scope, yylineno);
        emit(sub, $1, $3, $$, getcurrQuad() + 1, yylineno);
    }
    | expr OPERATOR_MOD expr    {
        printf("expr %% expr -> expr\n");
        $$ = newExpr(arithexpr_e);
        $$ -> symbol = newTemp(scope, yylineno);
        emit(mod, $1, $3, $$, getcurrQuad() + 1, yylineno);
    }
    | expr OPERATOR_DIV expr    {
        printf("expr / expr -> expr\n");
        $$ = newExpr(arithexpr_e);
        $$ -> symbol = newTemp(scope, yylineno);
        emit(divide, $1, $3, $$, getcurrQuad() + 1, yylineno);
    }
    | expr OPERATOR_MUL expr    {
        printf("expr * expr -> expr\n");
        $$ = newExpr(arithexpr_e);
        $$ -> symbol = newTemp(scope, yylineno);
        emit(mul, $1, $3, $$, getcurrQuad() + 1, yylineno);
    }
    | expr OPERATOR_GRT expr {
            printf("expr > expr -> expr\n");
            $$ = newExpr(boolexpr_e);
            $$ -> symbol = newTemp(scope, yylineno);

            $$ -> trueList = insertBoolStmt(getcurrQuad(), $$ -> trueList);
            emit(if_greater, $1, $3, NULL, 0, yylineno);

            $$ -> falseList = insertBoolStmt(getcurrQuad(), $$ -> falseList);
            emit(jump, NULL, NULL, NULL, 0, yylineno);

        }
    | expr OPERATOR_GRE expr    {
            printf("expr >= expr -> expr\n");
            $$ = newExpr(boolexpr_e);
            $$ -> symbol = newTemp(scope, yylineno);

            $$ -> trueList = insertBoolStmt(getcurrQuad(), $$ -> trueList);
            emit(if_greatereq, $1, $3, NULL, 0, yylineno);

            $$ -> falseList = insertBoolStmt(getcurrQuad(), $$ -> falseList);
            emit(jump, NULL, NULL, NULL, 0, yylineno);
        }
    | expr OPERATOR_LES expr{
            printf("expr < expr -> expr\n");
            $$ = newExpr(boolexpr_e);
            $$ -> symbol = newTemp(scope, yylineno);

            $$ -> trueList = insertBoolStmt(getcurrQuad(), $$ -> trueList);
            emit(if_less, $1, $3, NULL, 0, yylineno);

            $$ -> falseList = insertBoolStmt(getcurrQuad(), $$ -> falseList);
            emit(jump, NULL, NULL, NULL, 0, yylineno);
        } 
    | expr OPERATOR_LEE expr    {
            printf("expr <= expr -> expr\n");
            $$ = newExpr(boolexpr_e);
            $$ -> symbol = newTemp(scope, yylineno);

            $$ -> trueList = insertBoolStmt(getcurrQuad(), $$ -> trueList);
            emit(if_lesseq, $1, $3, NULL, 0, yylineno);

            $$ -> falseList = insertBoolStmt(getcurrQuad(), $$ -> falseList);
            emit(jump, NULL, NULL, NULL, 0, yylineno);
        }
    | expr OPERATOR_EQ expr 	{
            printf("expr == expr -> expr\n");

            $$ = newExpr(boolexpr_e);
            $$ -> symbol = newTemp(scope, yylineno);

            $$ -> trueList = insertBoolStmt(getcurrQuad(), $$ -> trueList);
            emit(if_eq, $1, $3, NULL, 0, yylineno);

            $$ -> falseList = insertBoolStmt(getcurrQuad(), $$ -> falseList);
            emit(jump, NULL, NULL, NULL, 0, yylineno);
        }
    | expr OPERATOR_NEQ expr    {
            printf("expr != expr -> expr\n");
            $$ = newExpr(boolexpr_e);
            $$ -> symbol = newTemp(scope, yylineno);

            $$ -> trueList = insertBoolStmt(getcurrQuad(), $$ -> trueList);
            emit(if_noteq, $1, $3, NULL, 0, yylineno);

            $$ -> falseList = insertBoolStmt(getcurrQuad(), $$ -> falseList);
            emit(jump, NULL, NULL, NULL, 0, yylineno);
        }
    | expr AND {
        if($1 -> exprType != boolexpr_e) {
            $1 -> trueList = insertBoolStmt(getcurrQuad(), $1 -> trueList);
            emit(if_eq, newExpr_constbool(1), NULL, $1, 0, yylineno);

            $1 -> falseList = insertBoolStmt(getcurrQuad(), $1 -> falseList);
            emit(jump, NULL, NULL, NULL, 0, yylineno);

        } 
    }M expr    {
        printf("expr and expr -> expr\n");
        $$ = newExpr(boolexpr_e);
        $$ -> symbol = newTemp(scope, yylineno);

        if($4 != 0) {
            patchBoolList($4, $1 -> trueList);
            emptyBoolList($1 -> trueList);
        } else {
            patchBoolList(getcurrQuad(), $1 -> trueList);
            emptyBoolList($1 -> trueList);
        }
        
        if($5 -> exprType != boolexpr_e) {

            $5 -> trueList = insertBoolStmt(getcurrQuad(), $5 -> trueList);
            emit(if_eq, newExpr_constbool(1), NULL, $5, 0, yylineno);

            $5 -> falseList = insertBoolStmt(getcurrQuad(), $5 -> falseList);
            emit(jump, NULL, NULL, NULL, 0, yylineno);
        }

        
        $$ -> trueList = $5 -> trueList;
        $$ -> falseList = mergeList($1 -> falseList, $5 -> falseList);
    } 
    | expr OR {
        if($1 -> exprType != boolexpr_e) {

            $1 -> trueList = insertBoolStmt(getcurrQuad(), $1 -> trueList);
            emit(if_eq, newExpr_constbool(1), NULL, $1, 0, yylineno);

            $1 -> falseList = insertBoolStmt(getcurrQuad(), $1 -> falseList);
            emit(jump, NULL, NULL, NULL, 0, yylineno);

        } 
    }M expr 		{
        printf("expr or expr -> expr\n");
        $$ = newExpr(boolexpr_e);
        $$ -> symbol = newTemp(scope, yylineno);

        if($4 != 0) {
            patchBoolList($4, $1 -> falseList);
            emptyBoolList($1 -> falseList);
        } else {
            patchBoolList(getcurrQuad(), $1 -> falseList);
            emptyBoolList($1 -> falseList);
        }
        
        if($5 -> exprType != boolexpr_e) {

            $5 -> trueList = insertBoolStmt(getcurrQuad(), $5 -> trueList);
            emit(if_eq, newExpr_constbool(1), NULL, $5, 0, yylineno);

            $5 -> falseList = insertBoolStmt(getcurrQuad(), $5 -> falseList);
            emit(jump, NULL, NULL, NULL, 0, yylineno);
        }

        
        $$ -> falseList = $5 -> falseList;
        $$ -> trueList = mergeList($1 -> trueList, $5 -> trueList);
    } 
    |term   {
            printf("term -> expr\n");
            $$ = $1;
        }  
    ;

assignexpr: lvalue {
                if(memberflag == 0){
                    SymbolTableEntry* temp;
                    temp = lookupScope(yylval.strVal, scope);
                    if(temp != NULL) {
                        if(temp -> type == USERFUNC || temp -> type == LIBFUNC) {
                            yyerror("\033[31mERROR: Function cannot be used as lvalue\033[0m\t");
                            return 1;
                        }
                    }
                }else{
                    memberflag = 0;
                }
            }
            OPERATOR_ASSIGN expr {
            printf("lvalue = expr -> assignexpr\n");
            if(!isEmptyBoolList($4 -> trueList) || !isEmptyBoolList($4 -> falseList)) {
                patchBoolList(getcurrQuad(), $4 -> trueList);
                patchBoolList(getcurrQuad() + 2, $4 -> falseList);
                emit(assign, newExpr_constbool(1), NULL, $4, getcurrQuad() + 1, yylineno);
                emit(jump, NULL, NULL, NULL, getcurrQuad() + 2, yylineno);
                emit(assign, newExpr_constbool(0), NULL, $4, getcurrQuad() + 1, yylineno);
                emptyBoolList($4 -> trueList);
                emptyBoolList($4 -> falseList);
            }
            if($1->exprType == tableitem_e) {
                emit(tablesetelem, $1->index, $4, $1, getcurrQuad()+1, yylineno );
                $$ = emit_ifTableItem($1,scope,yylineno);
                $$ -> exprType = assignexpr_e;
            }    
            else {
                emit(assign, $4, NULL, $1,getcurrQuad() + 1,yylineno);
                $$ = newExpr(assignexpr_e);
                $$->symbol = newTemp(scope,yylineno);
                emit(assign, $1, NULL, $$, getcurrQuad() + 1, yylineno);
            }
        }
    | call OPERATOR_ASSIGN expr {
        yyerror("\033[31mERROR: function call cannot be an lvalue\033[0m\t");
        return 1;
    }
    ;


primary: lvalue {
        printf("lvalue -> primary\n");
        $$ = emit_ifTableItem($1, scope, yylineno);
    }
    |call   {
            printf("call -> primary\n"); 
            $$ = (Expr*)malloc(sizeof(Expr));
            $$ = $1;
        }
    |objectdef  {printf("objectdef -> primary\n");}
    |LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS {
        printf("(funcdef) -> primary\n");
        $$ = $2;
    }
    |const  {printf("const -> primary\n");}
    ;

lvalue: ID  {
    int searchScope;
    SymbolTableEntry *temp, *f;
    printf("ID -> lvalue\n");
    if(funcFlag != 0) {
        f = lookupEverything(currFunc, scope);
        assert(f != NULL);
        searchScope = getEntryScope(f);
        temp = lookupEverything(yylval.strVal, searchScope + 1);
        if(temp != NULL) {
            if(getEntryScope(temp) != 0 && getEntryScope(temp) < (searchScope + 1) && temp -> type != USERFUNC && temp -> type != FORMAL) {
                    printf("\033[31mERROR: Cannot access \"%s\" inside function\033[0m", getEntryName(temp));
                    yyerror("\t");
                    return 1;
            }
        }
    }
    insertID(yylval.strVal, scope, yylineno);


    temp = lookupEverything(yylval.strVal, scope);
    assert(temp!=NULL);
    $$ = lvalue_expr(temp, scope, yylineno);

    }
    |LOCAL_KEYWORD ID   {
        printf("local ID -> lvalue\n");
        SymbolTableEntry *temp;
        insertLocalID(yylval.strVal, scope, yylineno);
        temp = lookupScope(yylval.strVal, scope);
        assert(temp!=NULL);
        $$ = lvalue_expr(temp, scope, yylineno);
    }
    |DOUBLE_COLON ID    {  
        printf("::ID -> lvalue\n");
        SymbolTableEntry *temp;
        temp = lookupScope(yylval.strVal, 0);
        if(lookupScope(yylval.strVal, 0) == NULL) {
            printf("\033[31mERROR: Global variable \"%s\" cannot be found!\033[0m", yylval.strVal);
            yyerror("\t");
            return 1;
        }
        if(temp!=NULL){
            $$ = lvalue_expr(temp, scope, yylineno);
        }
    }
    |member {
        printf("member -> lvalue\n");
        memberflag = 1;
        $$ = $1;
    }
    ;

member: lvalue DOT ID   {
        printf("lvalue.ID -> member\n");
        $$ = member_item($1, yylval.strVal, scope, yylineno); /*DANGER debug saving time*/
    }
    |lvalue LEFT_BRACE expr RIGHT_BRACE 
    {
        printf("lvalue[expr] -> member\n");
        $1 = emit_ifTableItem($1,scope,yylineno);
        $$ = newExpr(tableitem_e);
        $$ -> symbol = $1 -> symbol;
        $$ -> index = $3;
    }
    |call DOT ID    {
            printf("call.id -> member\n"); 
            $$ = member_item($1, yylval.strVal, scope, yylineno);
        } 
    |call LEFT_BRACE expr RIGHT_BRACE   {printf("call[expr] -> member\n");}
    ;

call: call LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {
        printf("call(elist) -> call\n");
        $$ = make_call($1, exprStack -> next, scope, yylineno);
        exprStack -> next = NULL;
    }
    |lvalue{
            SymbolTableEntry* temp;
            temp = lookupforCalls(getEntryName($1 -> symbol), scope);
            if(temp != NULL){
                if(getEntryScope(temp) != 0 && getEntryScope(temp) != scope && temp-> type  != USERFUNC) {
                    printf("\033[31mERROR: Cannot access \"%s\" inside function\033[0m", getEntryName(temp));
                    yyerror("\t");
                    return 1;
                }
            }
            
    } callsuffix  {
            printf("lvalue() -> call\n");
            $1 = emit_ifTableItem($1, scope, yylineno);
            if($3 -> method == 1) {
                Expr* t = (Expr*)malloc(sizeof(Expr));
                t = $1;
                $1 = emit_ifTableItem(member_item(t, $3 -> name, scope, yylineno), scope, yylineno);
                $3 -> e_list -> next = t;
            }
            
            $$ = make_call($1, $3 -> e_list, scope, yylineno);
            
        }
    |LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS elist RIGHT_PARENTHESIS
        {
            printf("(funcdef)(elist) -> call\n");
            Expr* func = (Expr*)malloc(sizeof(Expr));
            func = newExpr(programfunc_e);
            func -> symbol = $2 -> symbol;
            $$ = make_call(func, exprStack -> next, scope, yylineno);
            exprStack -> next = NULL;
        }
    ;

callsuffix: methodcall {
        printf("methodcall -> callsuffix\n");
        $$ = $1;
    }
    |normcall   {
        printf("normcall -> callsuffix\n");
        $$ = $1;
    }
    ;

normcall: LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {
        $$ = (callStruct*)malloc(sizeof(callStruct));
        $$ -> e_list = exprStack -> next;
        $$ -> method = 0;
        $$ -> name = NULL;
        exprStack -> next = NULL;
    }
    ;

methodcall: DOUBLE_DOT ID LEFT_PARENTHESIS elist RIGHT_PARENTHESIS  {
        printf("..id(elist) -> methodcall\n");
        $$ = (callStruct*)malloc(sizeof(callStruct));
        $$ -> e_list = exprStack -> next;
        $$ -> method = 1;
        $$ -> name = $2;
        exprStack -> next = NULL;
    }
    ;

elist: expr {
        if(exprStack -> next == NULL) {
            exprStack -> next = $1;
        }
        else {
            $1 -> next = exprStack -> next;
            exprStack -> next = $1;
        }

        if(!isEmptyBoolList($1 -> trueList) || !isEmptyBoolList($1 -> falseList)) {
            patchBoolList(getcurrQuad(), $1 -> trueList);
            patchBoolList(getcurrQuad() + 2, $1 -> falseList);
            
            emit(assign, newExpr_constbool(1), NULL, $1, getcurrQuad() + 1, yylineno);
            emit(jump, NULL, NULL, NULL, getcurrQuad() + 2, yylineno);
            emit(assign, newExpr_constbool(0), NULL, $1, getcurrQuad() + 1, yylineno);
            
            emptyBoolList($1 -> trueList);
            emptyBoolList($1 -> falseList);
            
        }
    }
    |expr COMMA elist {
        if(exprStack -> next == NULL) {
            exprStack -> next = $1;
        }
        else {
            $1 -> next = exprStack -> next;
            exprStack -> next = $1;
        }
    }
    |
    ;

objectdef: LEFT_BRACE elist RIGHT_BRACE {
        printf("[elist] -> objectdef\n");
        Expr * index;
        Expr* t = newExpr(newtable_e);
        int i = 0;
        t -> symbol = newTemp(scope, yylineno);
        emit(tablecreate, NULL, NULL, t, getcurrQuad() + 1, yylineno);
        index = exprStack -> next;
        while(index != NULL) {
            emit(tablesetelem, newExpr_constnum(i), index, t, getcurrQuad() + 1, yylineno);
            i++;
            index = index -> next;
        }
        $$ = t;
        exprStack-> next = NULL;
    }
    |LEFT_BRACE indexed RIGHT_BRACE {
        printf("[indexed] -> objectdef\n");
        Expr* index;
        Expr *t = newExpr(newtable_e);
        t -> symbol = newTemp(scope, yylineno);
        emit(tablecreate, NULL, NULL, t, getcurrQuad() + 1, yylineno);
        index = exprStack -> next;
        while(index != NULL) {
            emit(tablesetelem,  index->index, index-> indexedelem_value, t, getcurrQuad() + 1, yylineno);
            index = index -> next;
        }
        
        $$ = t;
        exprStack-> next = NULL;
    }
    ;

indexed: indexedelem {
    if(exprStack -> next == NULL) {
            exprStack -> next = $1;
        }
        else {
            $1 -> next = exprStack -> next;
            exprStack -> next = $1;
        }
    }
    |indexedelem COMMA indexed {
        if(exprStack -> next == NULL) {
            exprStack -> next = $1;
        }
        else {
            $1 -> next = exprStack -> next;
            exprStack -> next = $1;
        }
    }
    ;

indexedelem: LEFT_BRACKET expr COLON expr RIGHT_BRACKET {
        printf("{expr : expr} -> indexed elem\n");
        Expr* t = newExpr(tableitem_e);
        t -> index = $2;
        t -> indexedelem_value = $4;       
        $$ = t;
    }
    ;

block: LEFT_BRACKET {scope++;} set RIGHT_BRACKET {
        printf("block with stmts -> block\n");
        hideEntries(scope);
        scope--;
    } 
    |LEFT_BRACKET {scope++;} RIGHT_BRACKET   {
            printf("empty block -> block\n");
            hideEntries(scope);
            scope--;
        }
    ;

funcdef: FUNCTION ID {
        funcFlag++;
        insertOffsetStack(offsetStack, yylval.strVal, loopFlag);
        loopFlag = 0;
        currFunc = strdup(yylval.strVal);
        temp_func -> name = yylval.strVal;
        temp_func -> scope = scope;
        temp_func -> line = yylineno;
        enterScopeSpace();
        resetScopeOffset();
    }
    LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS  {
        
        SymbolTableEntry *temp;
        temp = lookupScope(temp_func -> name, temp_func -> scope);

        if(temp != NULL){
            if(temp -> type == LIBFUNC){
                printf("\033[31mERROR: Function name redefinition \"%s\" is a library function\033[0m", temp_func -> name);
                yyerror("\t");
                return 1;
            }

            if(temp -> type == USERFUNC){
                printf("\033[31mERROR: Function name redefinition \"%s\" is already in use\033[0m", temp_func -> name);
                yyerror("\t");
                return 1;
            }
            if(temp -> type == GLOBAL || temp -> type == LOCAL || temp -> type == FORMAL){
                printf("\033[31mERROR: Variable \"%s\" already defined\033[0m", temp_func -> name);
                yyerror("\t");
                return 1;
            }
        }
        else{
            SymbolTableEntry *new_entry;
            Function *new_func;
            int i;
            
            new_entry = (SymbolTableEntry *)malloc(sizeof(SymbolTableEntry));
            new_func =  (Function *)malloc(sizeof(Function));
            new_func -> arguments = (char**)malloc(10*sizeof(char *));

            new_func -> name = strdup(temp_func -> name);
            new_func-> label = getcurrQuad()+1;
            new_func -> scope = temp_func -> scope;
            new_func -> line = temp_func -> line;
            for(i = 0;i < arg_index;i++){
                new_func -> arguments[i] = strdup(temp_func -> arguments[i]);
                insertFormal(new_func -> arguments[i], temp_func -> scope + 1, yylineno);
            }
            new_entry -> isActive = 1;
            new_entry -> varVal = NULL;
            new_entry -> funcVal = new_func;
            new_entry -> type = USERFUNC;

            insertEntry(new_entry);
            $<exp>$ = lvalue_expr(new_entry, scope,yylineno);
            emit(jump, NULL, NULL, NULL, 0, yylineno);
            emit(funcstart,NULL, NULL,$<exp>$,getcurrQuad()+1, yylineno);


            temp_func -> name = "";
            temp_func -> scope = 0;
            temp_func -> line = 0;
            for(i = 0;i < arg_index;i++)
                temp_func -> arguments[i] = ""; 
            arg_index = 0;
        }
        enterScopeSpace();
        resetScopeOffset();
    }
    block    {
        printf("function id(idlist)block -> funcdef\n");
        MinasTirithTouSpitiouMou* tmp = (MinasTirithTouSpitiouMou*) malloc(sizeof(MinasTirithTouSpitiouMou));
        if(funcFlag >= 0){
            tmp = popoffsetStack(offsetStack);
            lushAlex -> symbol = lookupScope(tmp -> name, scope);
            loopFlag = tmp -> activeLoops;
            restoreformalArgs(tmp);
            restoreLocalVars(tmp);
        }
        $$ = lvalue_expr(lushAlex -> symbol, scope, yylineno);
        updateEntry(getEntryName(lushAlex->symbol),currScopeOffset(), getEntryScope(lushAlex->symbol));
        patchList(retStack -> next, getcurrQuad(), funcFlag);
        popSpecialScope(retStack, funcFlag);
        emit(funcend,NULL, NULL,$$,getcurrQuad()+1, yylineno);
        patchLabel(tmp -> jumpQuad, getcurrQuad());
        exitScopeSpace();
        exitScopeSpace();
        funcFlag--;
    }
    |FUNCTION {
        funcFlag++;
        char* fname = (char*) malloc(sizeof(char)*50);
        sprintf( fname, "_anonfunc%d", anonFuncCounter);
        currFunc = strdup(fname);
        anonFuncCounter++;
        insertOffsetStack(offsetStack, fname, loopFlag);
        loopFlag = 0;
        temp_func -> scope = scope;
        temp_func -> line = yylineno;
        temp_func -> name = fname;
        enterScopeSpace();
        resetScopeOffset();
        
    }LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS {
        SymbolTableEntry *temp;
        temp = lookupScope(temp_func -> name, temp_func -> scope);

        if(temp != NULL){
            if(temp -> type == LIBFUNC){
                printf("\033[31mERROR: Function name redefinition \"%s\" is a library function\033[0m", temp_func -> name);
                yyerror("\t");
                return 1;
            }

            if(temp -> type == USERFUNC){
                printf("\033[31mERROR: Function name redefinition \"%s\" is already in use\033[0m", temp_func -> name);
                yyerror("\t");
                return 1;
            }
            if(temp -> type == GLOBAL || temp -> type == LOCAL || temp -> type == FORMAL){
                printf("\033[31mERROR: Variable \"%s\" already defined\033[0m", temp_func -> name);
                yyerror("\t");
                return 1;
            }
            
        }
        else {
            SymbolTableEntry *new_entry;
            Function *new_func;
            int i;
            
            new_entry = (SymbolTableEntry *)malloc(sizeof(SymbolTableEntry));
            new_func =  (Function *)malloc(sizeof(Function));

            new_func -> arguments = (char**)malloc(10*sizeof(char *));
            new_func -> name = strdup(temp_func -> name);
            new_func-> label = getcurrQuad()+1;
            new_func -> scope = temp_func -> scope;
            new_func -> line = temp_func -> line;

            for(i = 0;i < arg_index;i++){
                new_func -> arguments[i] = (char*) malloc(sizeof(char)*100);
                new_func -> arguments[i] = strdup(temp_func -> arguments[i]);
                insertFormal(new_func -> arguments[i], temp_func -> scope + 1, yylineno);
            }

            new_entry -> isActive = 1;
            new_entry -> varVal = NULL;
            new_entry -> funcVal = new_func;
            new_entry -> type = USERFUNC;

            insertEntry(new_entry);
            $<exp>$ = lvalue_expr(new_entry, scope,yylineno);
            emit(jump, NULL, NULL, NULL, 0, yylineno);
            emit(funcstart,NULL, NULL,$<exp>$,getcurrQuad()+1, yylineno);
            
            temp_func -> name = "";
            temp_func -> scope = 0;
            temp_func -> line = 0;
            for(i = 0;i < arg_index;i++)
                temp_func -> arguments[i] = "";
            arg_index = 0;
            
        }
        enterScopeSpace();
        resetScopeOffset();

    } block    {
        printf("function (idlist)block -> funcdef\n");
        MinasTirithTouSpitiouMou* tmp = (MinasTirithTouSpitiouMou*) malloc(sizeof(MinasTirithTouSpitiouMou));
        if(funcFlag >= 0){
            tmp = popoffsetStack(offsetStack);
            lushAlex -> symbol = lookupScope(tmp -> name, scope);
            loopFlag = tmp -> activeLoops;
            restoreformalArgs(tmp);
            restoreLocalVars(tmp);
        }
        $$ = lvalue_expr(lushAlex -> symbol, scope, yylineno);
        updateEntry(getEntryName(lushAlex -> symbol), currScopeOffset(), getEntryScope(lushAlex -> symbol));
        patchList(retStack -> next, getcurrQuad(), funcFlag);
        popSpecialScope(retStack, funcFlag);
        emit(funcend, NULL, NULL , $$, getcurrQuad() + 1, yylineno);
        patchLabel(tmp -> jumpQuad, getcurrQuad());
        exitScopeSpace();
        exitScopeSpace();
        funcFlag--;
    }
    ;

const: REAL {$$ = newExpr_constnum(yylval.doubleVal);}
    |INTEGER {$$ = newExpr_constnum(yylval.intVal);}
    |STRING  {$$ = newExpr_conststring(yylval.strVal);}
    |NIL    {$$ = newExpr(nil_e);}
    |TRUE   {$$ = newExpr_constbool(1);}
    |FALSE  {$$ = newExpr_constbool(0);}
    ;

idlist: ID {
        printf("id -> idlist\n");
        int j;
        int flag = 1;
        
        for(j = 0; j < arg_index; j++){
            flag = strcmp(yylval.strVal, temp_func -> arguments[j]);
            if(flag == 0) 
                break;
        }

        if(flag == 0) {
            printf("\033[31mERROR: Symbol with name \"%s\" already exists!\033[0m\t", yylval.strVal);
            yyerror("\t");
            return 1;
        }else {
            temp_func -> arguments[arg_index] = yylval.strVal;
            arg_index++;
        }
        
        
    }
    |ID {
        int j;
        int flag = 1;
        
        for(j = 0; j < arg_index; j++){
            flag = strcmp(yylval.strVal, temp_func -> arguments[j]);
            if(flag == 0) 
                break;
        }

        if(flag == 0) {
            printf("\033[31mERROR: Symbol with name \"%s\" already exists!\033[0m\t", yylval.strVal);
            yyerror("\t");
            return 1;
        }else {
            temp_func -> arguments[arg_index] = yylval.strVal;
            arg_index++;
        }
    }
    COMMA idlist {printf(", idlist -> idlist\n");}
    |{printf("empty -> idlist\n");}
    ;

ifprefix: IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS {
    if(!isEmptyBoolList($3 -> trueList) || !isEmptyBoolList($3 -> falseList)) {
            patchBoolList(getcurrQuad(), $3 -> trueList);
            patchBoolList(getcurrQuad() + 2, $3 -> falseList);
            
            emit(assign, newExpr_constbool(1), NULL, $3, getcurrQuad() + 1, yylineno);
            emit(jump, NULL, NULL, NULL, getcurrQuad() + 2, yylineno);
            emit(assign, newExpr_constbool(0), NULL, $3, getcurrQuad() + 1, yylineno);
            
            emptyBoolList($3 -> trueList);
            emptyBoolList($3 -> falseList);
            
        }

    emit(if_eq, newExpr_constbool(1), NULL, $3, getcurrQuad() + 2, yylineno);
    $$ = getcurrQuad();
    emit(jump, NULL, NULL, NULL, 0, yylineno);
}
    ;

elseprefix: ELSE {
        $$ = getcurrQuad();
        emit(jump, NULL, NULL, NULL, 0, yylineno);
    }
    ;

ifstmt: ifprefix stmt   { 
        printf("if(expr) -> ifstmt\n");
        patchLabel($1, getcurrQuad());
    }
    |ifprefix stmt elseprefix stmt    {
            printf("if(expr) else -> ifstmt\n");
            patchLabel($1, $3 + 1);
            patchLabel($3, getcurrQuad());
        }
    ;

whilestart: WHILE {loopFlag++; $$ = getcurrQuad();}
    ;

whilecond: LEFT_PARENTHESIS expr RIGHT_PARENTHESIS {
        if(!isEmptyBoolList($2 -> trueList) || !isEmptyBoolList($2 -> falseList)) {
            patchBoolList(getcurrQuad(), $2 -> trueList);
            patchBoolList(getcurrQuad() + 2, $2 -> falseList);
            
            emit(assign, newExpr_constbool(1), NULL, $2, getcurrQuad() + 1, yylineno);
            emit(jump, NULL, NULL, NULL, getcurrQuad() + 2, yylineno);
            emit(assign, newExpr_constbool(0), NULL, $2, getcurrQuad() + 1, yylineno);
            
            emptyBoolList($2 -> trueList);
            emptyBoolList($2 -> falseList);
            
        }
        emit(if_eq, newExpr_constbool(1), NULL,  $2, getcurrQuad() + 2, yylineno);
        $$ = getcurrQuad();
        emit(jump, NULL, NULL, NULL, 0, yylineno);
    }
    ;

whilestmt: whilestart whilecond stmt   {
        printf("while(expr) -> whilestmt\n");
        emit(jump, NULL, NULL, NULL, $1, yylineno);
        patchLabel($2, getcurrQuad());

        patchList(breakStack -> next, getcurrQuad(), loopFlag);
        patchList(continueStack -> next, $1, loopFlag);
        popSpecialScope(continueStack, loopFlag);
        popSpecialScope(breakStack, loopFlag);
        loopFlag--;

    }
    ;    


N: {$$ = getcurrQuad(); emit(jump, NULL, NULL, NULL, 0, yylineno);}
;

M: {$$ = getcurrQuad();}
;

forprefix: FOR LEFT_PARENTHESIS elist SEMICOLON M expr SEMICOLON {
    loopFlag++;
    
    if(!isEmptyBoolList($6 -> trueList) || !isEmptyBoolList($6 -> falseList)) {
            patchBoolList(getcurrQuad(), $6 -> trueList);
            patchBoolList(getcurrQuad() + 2, $6 -> falseList);
            
            emit(assign, newExpr_constbool(1), NULL, $6, getcurrQuad() + 1, yylineno);
            emit(jump, NULL, NULL, NULL, getcurrQuad() + 2, yylineno);
            emit(assign, newExpr_constbool(0), NULL, $6, getcurrQuad() + 1, yylineno);
            
            emptyBoolList($6 -> trueList);
            emptyBoolList($6 -> falseList);
            
        }
    
    $$ = (loopStruct*)malloc(sizeof(loopStruct));
    $$ -> test = $5;
    $$ -> enter = getcurrQuad();

    emit(if_eq, newExpr_constbool(1), NULL, $6, 0, yylineno);
}


forstmt: forprefix N elist {exprStack -> next = NULL;} RIGHT_PARENTHESIS N stmt N {
            printf("for(elist;expr;elist)stmt -> forstmt\n"); 

            patchLabel($1 -> enter, $6 + 1);
            patchLabel($2, getcurrQuad());
            patchLabel($6, $1 -> test);
            patchLabel($8, $2 + 1);

            patchList(breakStack -> next,getcurrQuad(), loopFlag);
            patchList(continueStack -> next , $2 + 1, loopFlag);
            popSpecialScope(continueStack, loopFlag);
            popSpecialScope(breakStack, loopFlag);
            loopFlag--;
        }
    ;

returnstmt: RETURN expr SEMICOLON   {
        printf("return expr ; -> returnstmt\n");
        emit(ret, NULL, NULL, $2, getcurrQuad() + 1, yylineno);
        insertSpecialStmt(getcurrQuad(), funcFlag, retStack);
        emit(jump,NULL,NULL,NULL,0,yylineno);
    } 
    |RETURN SEMICOLON    {
        printf("return ; -> returnstmt\n");
        emit(ret, NULL, NULL, NULL, getcurrQuad() + 1, yylineno);
        insertSpecialStmt(getcurrQuad(), funcFlag, retStack);
        emit(jump,NULL,NULL,NULL,0,yylineno);

        }
    ;
%%


int yyerror(char *message){
    printf("%s: in line %d\n", message, yylineno); 
}

int main(int argc, char* argv[]){
    int found_error = 0;

    temp_func = (Function *)malloc(sizeof(Function));
    temp_func -> arguments =(char**)malloc(10*sizeof(char*));
    
    exprStack = (Expr*)malloc(sizeof(Expr));
    exprStack -> next = NULL;

    offsetStack = (MinasTirithTouSpitiouMou*) malloc(sizeof(MinasTirithTouSpitiouMou));
    offsetStack-> next = NULL;
    
    lushAlex = (Expr*)malloc(sizeof(Expr));

    breakStack = (specialStmt*) malloc(sizeof(specialStmt));
    breakStack -> next = NULL;

    continueStack = (specialStmt*) malloc(sizeof(specialStmt));
    continueStack -> next = NULL;

    retStack = (specialStmt*) malloc(sizeof(specialStmt));
    retStack -> next = NULL;



    initTable();

    found_error = yyparse();

    //printEntries();
    
    if(!found_error) {
        printQuads();
    }

    return 0;
}