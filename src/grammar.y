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

    char* currFunc;

    Expr* exprStack = (Expr *) 0;

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


%left SEMICOLON COLON COMMA DOUBLE_COLON
%left LEFT_BRACKET RIGHT_BRACKET
%left LEFT_BRACE RIGHT_BRACE
%left LEFT_PARENTHESIS RIGHT_PARENTHESIS

%right OPERATOR_ASSIGN
%left OR
%left AND

%nonassoc OPERATOR_EQ OPERATOR_NEQ
%right OPERATOR_GRT OPERATOR_LES OPERATOR_GRE OPERATOR_LEE
%left OPERATOR_PLUS OPERATOR_MINUS
%left OPERATOR_MUL OPERATOR_DIV OPERATOR_MOD
%right NOT OPERATOR_PP OPERATOR_MM

%left DOT DOUBLE_DOT    

%%
program: set   {printf("set -> program\n");}
    |   {printf("EMPTY -> program\n");}
    ;

set: stmt
    |set stmt
    ;

stmt: expr SEMICOLON    {printf("expr ; -> stmt\n");}
    |ifstmt {printf("ifstmt -> stmt\n");}
    |whilestmt  {printf("whilestmt -> stmt\n");}
    |forstmt    {printf("forstmt -> stmt\n");}
    |returnstmt    {
                printf("returnstmt -> stmt\n");
                if(funcFlag == 0) {
                    yyerror("\033[31mERROR: return statement outside function\033[0m\t");
                }
            }
    |BREAK SEMICOLON    {
            printf("break; -> stmt\n");
            if(loopFlag == 0) {
                yyerror("\033[31mERROR: break statement outside loop\033[0m\t");
            }
        }
    |CONTINUE SEMICOLON {
            printf("continue; -> stmt\n");
            if(loopFlag == 0) {
                yyerror("\033[31mERROR: continue statement outside loop\033[0m\t");
            }
        }
    |block  {printf("block -> stmt\n");}
    |funcdef    {printf("funcdef -> stmt\n");}
    |SEMICOLON  {printf("; -> stmt\n");}
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
        printf("expr %% expr -> expr\n");$$ = newExpr(arithexpr_e);
        $$ -> symbol = newTemp(scope, yylineno);
        emit(mod, $1, $3, $$, getcurrQuad() + 1, yylineno);
    }
    | expr OPERATOR_DIV expr    {
        printf("expr / expr -> expr\n");$$ = newExpr(arithexpr_e);
        $$ -> symbol = newTemp(scope, yylineno);
        emit(divide, $1, $3, $$, getcurrQuad() + 1, yylineno);
    }
    | expr OPERATOR_MUL expr    {
        printf("expr * expr -> expr\n");
        $$ = newExpr(arithexpr_e);
        $$ -> symbol = newTemp(scope, yylineno);
        emit(mul, $1, $3, $$, getcurrQuad() + 1, yylineno);
    }
    | expr OPERATOR_GRT expr    {printf("expr > expr -> expr\n");}
    | expr OPERATOR_GRE expr    {printf("expr >= expr -> expr\n");}
    | expr OPERATOR_LES expr    {printf("expr < expr -> expr\n");}
    | expr OPERATOR_LEE expr    {printf("expr <= expr -> expr\n");}
    | expr OPERATOR_EQ expr 	{printf("expr == expr -> expr\n");}
    | expr OPERATOR_NEQ expr    {printf("expr != expr -> expr\n");}
    | expr AND expr    		{
        printf("expr and expr -> expr\n");
        $$ = newExpr(boolexpr_e);
        $$ -> symbol = newTemp(scope, yylineno);
        emit(and, $1, $3, $$, getcurrQuad() + 1, yylineno);
    }
    | expr OR expr 		{
        printf("expr or expr -> expr\n");
        $$ = newExpr(boolexpr_e);
        $$ -> symbol = newTemp(scope, yylineno);
        emit(or, $1, $3, $$, getcurrQuad() + 1, yylineno);
    }
    |term   {printf("term -> expr\n");}  
    ;

term: LEFT_PARENTHESIS expr RIGHT_PARENTHESIS   {
        printf("(expr) -> term");
        $$=$2;
    }
    |OPERATOR_MINUS expr    {
        printf("- expr -> term\n");
        if(checkArith($2) == 1){
            $$ = newExpr(arithexpr_e);
            $$ -> symbol = newTemp(scope, yylineno);
            emit(uminus, $2, NULL, $$, getcurrQuad()+1, yylineno);
        }else {
            printf("\033[31mERROR: unary minus expression used outside arithmetic expression \033[0m \n");
        }
    }
    |NOT expr  		{
            printf("not expr -> term\n");
            $$ = newExpr(boolexpr_e);
            $$ -> symbol = newTemp(scope, yylineno);
            emit(not, $2, NULL, $$, getcurrQuad()+1, yylineno);
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
        }

        }
    |primary    {
        printf("primary -> term\n");
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
                        }
                    }
                }else{
                    memberflag = 0;
                }
            }
            OPERATOR_ASSIGN expr {
            printf("lvalue = expr -> assignexpr\n");

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
    }
    ;

primary: lvalue {printf("lvalue -> primary\n");
                    $$ = emit_ifTableItem($1, scope, yylineno);
    }
    |call   {printf("call -> primary\n");}
    |objectdef  {printf("objectdef -> primary\n");}
    |LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS {printf("(funcdef) -> primary\n");}
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
            if(getEntryScope(temp) != 0 && getEntryScope(temp) < (searchScope + 1) && temp -> type != USERFUNC) {
                    printf("\033[31mERROR: Cannot access \"%s\" inside function\033[0m", getEntryName(temp));
                    yyerror("\t");
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
    |call DOT ID    {printf("call.id -> member\n");}
    |call LEFT_BRACE expr RIGHT_BRACE   {printf("call[expr] -> member\n");}
    ;

call: call LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {printf("call(elist) -> call\n");}
    |lvalue{
            SymbolTableEntry* temp;
            temp = lookupforCalls(yylval.strVal, scope);
            if(temp != NULL){
                if(getEntryScope(temp) != 0 && getEntryScope(temp) != scope && temp-> type  != USERFUNC) {
                    printf("\033[31mERROR: Cannot access \"%s\" inside function\033[0m", getEntryName(temp));
                    yyerror("\t");
                }
            }
            
    } callsuffix  {
            printf("lvalue() -> call\n");
        }
    |LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS elist RIGHT_PARENTHESIS
        {printf("(funcdef)(elist) -> call\n");}
    ;

callsuffix: methodcall {
        printf("methodcall -> callsuffix\n");
    }
    |normcall   {printf("normcall -> callsuffix\n");}
    ;

normcall: LEFT_PARENTHESIS elist RIGHT_PARENTHESIS
    ;

methodcall: DOUBLE_DOT ID normcall  {printf("..id(elist) -> methodcall\n");}
    ;

elist: expr {
        if(exprStack -> next == NULL) {
            exprStack -> next = $1;
        }
        else {
            $1 -> next = exprStack -> next;
            exprStack -> next = $1;
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
        insertOffsetStack(offsetStack, yylval.strVal);

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
            }

            if(temp -> type == USERFUNC){
                printf("\033[31mERROR: Function name redefinition \"%s\" is already in use\033[0m", temp_func -> name);
                yyerror("\t");
            }
            if(temp -> type == GLOBAL || temp -> type == LOCAL || temp -> type == FORMAL){
                printf("\033[31mERROR: Variable \"%s\" already defined\033[0m", temp_func -> name);
                yyerror("\t");
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
        printf("function id(idlist)block -> funcdef\n", yytext);
        funcFlag--;
        if(funcFlag >= 0){
            MinasTirithTouSpitiouMou* tmp = (MinasTirithTouSpitiouMou*) malloc(sizeof(MinasTirithTouSpitiouMou));
            tmp = popoffsetStack(offsetStack);
            lushAlex -> symbol = lookupScope(tmp -> name, scope);
            printf("%s \n", getEntryName(lushAlex -> symbol));
            restoreformalArgs(tmp);
            restoreLocalVars(tmp);
        }
        $$ = lvalue_expr(lushAlex -> symbol, scope, yylineno);
        updateEntry(getEntryName(lushAlex->symbol),currScopeOffset(), getEntryScope(lushAlex->symbol));
        emit(funcend,NULL, NULL,$$,getcurrQuad()+1, yylineno);
        exitScopeSpace();
        exitScopeSpace();
    }
    |FUNCTION {
        funcFlag++;
         
        char* fname = (char*) malloc(sizeof(char)*50);
        sprintf( fname, "_anonfunc%d", anonFuncCounter);
        currFunc = strdup(fname);
        anonFuncCounter++;
        insertOffsetStack(offsetStack, fname);
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
            }

            if(temp -> type == USERFUNC){
                printf("\033[31mERROR: Function name redefinition \"%s\" is already in use\033[0m", temp_func -> name);
                yyerror("\t");
            }
            if(temp -> type == GLOBAL || temp -> type == LOCAL || temp -> type == FORMAL){
                printf("\033[31mERROR: Variable \"%s\" already defined\033[0m", temp_func -> name);
                yyerror("\t");
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
        funcFlag--;
        if(funcFlag >= 0){
            MinasTirithTouSpitiouMou* tmp = (MinasTirithTouSpitiouMou*) malloc(sizeof(MinasTirithTouSpitiouMou));
            tmp = popoffsetStack(offsetStack);
            lushAlex -> symbol = lookupScope(tmp -> name, scope);
            printf("%s \n", getEntryName(lushAlex -> symbol));
            restoreformalArgs(tmp);
            restoreLocalVars(tmp);
        }
        $$ = lvalue_expr(lushAlex -> symbol, scope, yylineno);
        updateEntry(getEntryName(lushAlex->symbol),currScopeOffset(), getEntryScope(lushAlex->symbol));
        emit(funcend,NULL, NULL,$$,getcurrQuad()+1, yylineno);
        exitScopeSpace();
        exitScopeSpace();
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
        }else {
            temp_func -> arguments[arg_index] = yylval.strVal;
            arg_index++;
        }
    }
    COMMA idlist {printf(", idlist -> idlist\n");}
    |{printf("empty -> idlist\n");}
    ;

ifstmt: IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt   {printf("if(expr) -> ifstmt\n");}
    |IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt ELSE stmt    {printf("if(expr) else -> ifstmt\n");}
    ;

whilestmt: WHILE {loopFlag++;} LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt   {printf("while(expr) -> whilestmt\n"); loopFlag--;}
    ;    

forstmt: FOR {loopFlag++;} LEFT_PARENTHESIS elist SEMICOLON expr SEMICOLON elist RIGHT_PARENTHESIS stmt
        {printf("for(elist;expr;elist)stmt -> forstmt\n"); loopFlag--;}
    ;

returnstmt: RETURN expr SEMICOLON   {printf("return expr ; -> returnstmt\n");} 
    |RETURN SEMICOLON    {printf("return ; -> returnstmt\n");}
    ;
%%


int yyerror(char *message){
    printf("%s: in line %d\n", message, yylineno); 
}

int main(int argc, char* argv[]){

    temp_func = (Function *)malloc(sizeof(Function));
    temp_func -> arguments =(char**)malloc(10*sizeof(char*));
    exprStack = (Expr*)malloc(sizeof(Expr));
    offsetStack = (MinasTirithTouSpitiouMou*) malloc(sizeof(MinasTirithTouSpitiouMou));
    lushAlex = (Expr*)malloc(sizeof(Expr));
    exprStack -> next = NULL;
    offsetStack-> next = NULL;

    initTable();

    yyparse();

    //printEntries();

    printQuads();

    return 0;
}