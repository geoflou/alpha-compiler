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

    Function* temp_func;
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
}


%start program

%expect 1

%token <strVal> ID
%token <intVal> INTEGER
%token <doubleVal> REAL
%token <strVal> STRING

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

expr: assignexpr    {printf("assignexpr -> expr\n");}
    | expr OPERATOR_PLUS expr   {printf("expr + expr -> expr\n");}
    | expr OPERATOR_MINUS expr  {printf("expr - expr -> expr\n");}
    | expr OPERATOR_MOD expr    {printf("expr % expr -> expr\n");}
    | expr OPERATOR_DIV expr    {printf("expr / expr -> expr\n");}
    | expr OPERATOR_MUL expr    {printf("expr * expr -> expr\n");}
    | expr OPERATOR_GRT expr    {printf("expr > expr -> expr\n");}
    | expr OPERATOR_GRE expr    {printf("expr >= expr -> expr\n");}
    | expr OPERATOR_LES expr    {printf("expr < expr -> expr\n");}
    | expr OPERATOR_LEE expr    {printf("expr <= expr -> expr\n");}
    | expr OPERATOR_EQ expr 	{printf("expr == expr -> expr\n");}
    | expr OPERATOR_NEQ expr    {printf("expr != expr -> expr\n");}
    | expr AND expr    		{printf("expr and expr -> expr\n");}
    | expr OR expr 		{printf("expr or expr -> expr\n");}
    |term   {printf("term -> expr\n");}  
    ;

term: LEFT_PARENTHESIS expr RIGHT_PARENTHESIS   {printf("(expr) -> term");}
    |OPERATOR_MINUS expr    {printf("- expr -> term\n");}
    |NOT expr  		{printf("not expr -> term\n");}
    |OPERATOR_PP lvalue {
            printf("++lvalue -> term\n");
            SymbolTableEntry* temp;
            temp = lookupScope(yylval.strVal, scope);
            if(temp != NULL) {
                if(temp -> type == USERFUNC || temp -> type == LIBFUNC) {
                    yyerror("\033[31mERROR: Function cannot be used as lvalue\033[0m\t");
                }
            }
        }
    |lvalue {
                SymbolTableEntry* temp;
                temp = lookupScope(yylval.strVal, scope);
                if(temp != NULL) {
                    if(temp -> type == USERFUNC || temp -> type == LIBFUNC) {
                        yyerror("\033[31mERROR: Function cannot be used as lvalue\033[0m\t");
                    }
                }
    }   OPERATOR_PP {printf("lvalue++ -> term\n");}
    |OPERATOR_MM lvalue {
            printf("--lvalue -> term\n");
            SymbolTableEntry* temp;
            temp = lookupScope(yylval.strVal, scope);
            if(temp != NULL) {
                if(temp -> type == USERFUNC || temp -> type == LIBFUNC) {
                    yyerror("\033[31mERROR: Function cannot be used as lvalue\033[0m\t");
                }
            }
        }
    |lvalue {
            printf("++lvalue -> term\n");
            SymbolTableEntry* temp;
            temp = lookupScope(yylval.strVal, scope);
            if(temp != NULL) {
                if(temp -> type == USERFUNC || temp -> type == LIBFUNC) {
                    yyerror("\033[31mERROR: Function cannot be used as lvalue\033[0m\t");
                }
            }
        } OPERATOR_MM {
            printf("lvalue-- -> term\n");

        }
    |primary    {printf("primary -> term\n");}
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
            
        }
    | call OPERATOR_ASSIGN expr {
        yyerror("\033[31mERROR: function call cannot be an lvalue\033[0m\t");
    }
    ;

primary: lvalue {printf("lvalue -> primary\n");}
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
            printf("EDW %d\n", searchScope);
            if(getEntryScope(temp) != 0 && getEntryScope(temp) <= (searchScope) && temp -> type  != USERFUNC) {
                    printf("\033[31mERROR: Cannot access \"%s\" inside function\033[0m", getEntryName(temp));
                    yyerror("\t");
            }
        }
    }
    insertID(yylval.strVal, scope, yylineno);

    }
    |LOCAL_KEYWORD ID   {
        printf("local ID -> lvalue\n");
        insertLocalID(yylval.strVal, scope, yylineno);
    }
    |DOUBLE_COLON ID    {  
        printf("::ID -> lvalue\n");
        if(lookupScope(yylval.strVal, 0) == NULL) {
            printf("\033[31mERROR: Global variable \"%s\" cannot be found!\033[0m", yylval.strVal);
            yyerror("\t");
        }   
    }
    |member {
        printf("member -> lvalue\n");
        memberflag = 1;
    }
    ;

member: lvalue DOT ID   {printf("lvalue.ID -> mebmer\n");}
    |lvalue LEFT_BRACE expr RIGHT_BRACE {printf("lvalue[expr] -> member\n");}
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

elist: expr
    |expr COMMA elist
    |
    ;

objectdef: LEFT_BRACE elist RIGHT_BRACE {printf("[elist] -> objectdef\n");}
    |LEFT_BRACE indexed RIGHT_BRACE {printf("[indexed] -> objectdef\n]");}
    ;

indexed: indexedelem
    |indexedelem COMMA indexed
    ;

indexedelem: LEFT_BRACKET expr COLON expr RIGHT_BRACKET {printf("{expr : expr} -> indexed elem\n");}
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
        currFunc = strdup(yylval.strVal);
        temp_func -> name = yylval.strVal;
        temp_func -> scope = scope;
        temp_func -> line = yylineno;
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
            new_func -> scope = temp_func -> scope;
            new_func -> line = temp_func -> line;
            for(i = 0;i < arg_index;i++){
                new_func -> arguments[i] = strdup(temp_func -> arguments[i]);
                insertFormal(new_func -> arguments[i], scope + 1, yylineno);
            }
            new_entry -> isActive = 1;
            new_entry -> varVal = NULL;
            new_entry -> funcVal = new_func;
            new_entry -> type = USERFUNC;

            insertEntry(new_entry);
        }

    }
    block    {
        printf("function id(idlist)block -> funcdef\n", yytext);
        
        int i = 0;
        temp_func -> name = "";
        temp_func -> scope = 0;
        temp_func -> line = 0;
        for(i = 0;i < arg_index;i++)
            temp_func -> arguments[i] = ""; 
        arg_index = 0;
        funcFlag--;
    }
    |FUNCTION {
        funcFlag++;
        char* fname = (char*) malloc(sizeof(char)*50);
        sprintf( fname, "_anonfunc%d", anonFuncCounter);
        currFunc = strdup(fname);
        anonFuncCounter++;
        temp_func -> scope = scope;
        temp_func -> line = yylineno;
        temp_func -> name = fname;
        
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
            new_func -> scope = temp_func -> scope;
            new_func -> line = temp_func -> line;

            for(i = 0;i < arg_index;i++){
                new_func -> arguments[i] = (char*) malloc(sizeof(char)*100);
                new_func -> arguments[i] = strdup(temp_func -> arguments[i]);
                insertFormal(new_func -> arguments[i], scope + 1, yylineno);
            }

            new_entry -> isActive = 1;
            new_entry -> varVal = NULL;
            new_entry -> funcVal = new_func;
            new_entry -> type = USERFUNC;

            insertEntry(new_entry);
        }

    } block    {
        printf("function (idlist)block -> funcdef\n");

        int i = 0;
        temp_func -> name = "";
        temp_func -> scope = 0;
        temp_func -> line = 0;
        for(i = 0;i < arg_index;i++)
            temp_func -> arguments[i] = "";
        arg_index = 0;
        funcFlag--; 
    }
    ;

const: REAL
    |INTEGER
    |STRING 
    |NIL
    |TRUE
    |FALSE
    ;

idlist: ID {
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
    COMMA idlist
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


    initTable();

    yyparse();

    printEntries();

    return 0;
}