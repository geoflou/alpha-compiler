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

    Function* temp_func;
    int arg_index = 0;

    int anonFuncCounter = 0;

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
    |returnstmt    {printf("returnstmt -> stmt\n");}
    |BREAK SEMICOLON    {printf("break; -> stmt\n");}
    |CONTINUE SEMICOLON {printf("continue; -> stmt\n");}
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
    |OPERATOR_PP lvalue {printf("++lvalue -> term\n");}
    |lvalue OPERATOR_PP {printf("lvalue++ -> term\n");}
    |OPERATOR_MM lvalue {printf("--lvalue -> term\n");}
    |lvalue OPERATOR_MM {printf("lvalue-- -> term\n");}
    |primary    {printf("primary -> term\n");}
    ;

assignexpr: lvalue OPERATOR_ASSIGN expr {
        printf("lvalue = expr -> assignexpr\n");
    }
    ;

primary: lvalue {printf("lvalue -> primary\n");}
    |call   {printf("call -> primary\n");}
    |objectdef  {printf("objectdef -> primary\n");}
    |LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS {printf("(funcdef) -> primary\n");}
    |const  {printf("const -> primary\n");}
    ;

lvalue: ID  {
    printf("ID -> lvalue\n");
    insertID(yylval.strVal, scope, yylineno);
}

    |LOCAL_KEYWORD ID   {
        printf("local ID -> lvalue\n");
        insertLocalID(yylval.strVal, scope, yylineno);
    }
    |DOUBLE_COLON ID    {  
        printf("::ID -> lvalue\n");
        if(lookupScope(yylval.strVal, 0) == NULL) {
            yyerror("ERROR: Global variable cannot be found!\t");
        }   
    }
    |member {
        printf("member -> lvalue\n");
    }
    ;

member: lvalue DOT ID   {printf("lvalue.ID -> mebmer\n");}
    |lvalue LEFT_BRACE expr RIGHT_BRACE {printf("lvalue[expr] -> member\n");}
    |call DOT ID    {printf("call.id -> member\n");}
    |call LEFT_BRACE expr RIGHT_BRACE   {printf("call[expr] -> member\n");}
    ;

call: call LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {printf("call(elist) -> call\n");}
    |lvalue callsuffix  {printf("lvalue() -> call\n");}
    |LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS elist RIGHT_PARENTHESIS
        {printf("(funcdef)(elist) -> call\n");}
    ;

callsuffix: methodcall {printf("methodcall -> callsuffix\n");}
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
        temp_func -> name = yylval.strVal;
        temp_func -> scope = scope;
        temp_func -> line = yylineno;
    }
    LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS  {
        SymbolTableEntry *temp;
        temp = lookupScope(temp_func -> name, temp_func -> scope);

        if(temp == NULL){
            SymbolTableEntry *new_entry;
            Function *new_func;
            int i;
            
            new_entry = (SymbolTableEntry *)malloc(sizeof(SymbolTableEntry));
            new_func =  (Function *)malloc(sizeof(Function));
            new_func -> arguments = (char**)malloc(10*sizeof(char *));

            new_func -> name = strdup(temp_func -> name);
            new_func -> scope = temp_func -> scope;
            new_func -> line = temp_func -> line;
            for(i = 0;i < arg_index;i++)
                new_func -> arguments[i] = strdup(temp_func -> arguments[i]);

            new_entry -> isActive = 1;
            new_entry -> varVal = NULL;
            new_entry -> funcVal = new_func;
            new_entry -> type = USERFUNC;

            insertEntry(new_entry);
        }
        else{
            if(temp -> type == LIBFUNC){
                printf("ERROR: FUNCTION NAME REDEFINITION! %s IS A LIBRARY FUNCTION", temp_func -> name);
                yyerror("\t");
            }

            if(temp -> type == USERFUNC){
                printf("ERROR: FUNCTION NAME REDEFINITION %s IS ALREADY IN USE", temp_func -> name);
                yyerror("\t");
            }
            else{
                printf("ERROR: Variable %s already defined", temp_func -> name);
                yyerror("\t");
            }
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
        
    }
    |FUNCTION {
        char* fname = (char*) malloc(sizeof(char)*50);
        sprintf( fname, "_anonfunc%d", anonFuncCounter);
        anonFuncCounter++;
        temp_func -> scope = scope;
        temp_func -> line = yylineno;
        temp_func -> name = fname;
        
    }LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS {
        SymbolTableEntry *temp;
        temp = lookupScope(temp_func -> name, temp_func -> scope);

        if(temp == NULL){
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
            }

            new_entry -> isActive = 1;
            new_entry -> varVal = NULL;
            new_entry -> funcVal = new_func;
            new_entry -> type = USERFUNC;

            insertEntry(new_entry);
        }
        else{
            
            if(temp -> type == LIBFUNC){
                printf("ERROR: FUNCTION NAME REDEFINITION! %s IS A LIBRARY FUNCTION", temp_func -> name);
                yyerror("\t");
            }

            if(temp -> type == USERFUNC){
                printf("ERROR: FUNCTION NAME REDEFINITION %s IS ALREADY IN USE", temp_func -> name);
                yyerror("\t");
            }
            else{
                printf("ERROR: Variable %s already defined", temp_func -> name);
                yyerror("\t");
            }
        }

    } block    {
        printf("function (idlist)block -> funcdef\n");

        int i = 0;
        temp_func -> name = "";
        temp_func -> scope = 0;
        temp_func -> line = 0;
        for(i = 0;i < arg_index;i++)
        temp_func -> arguments[i] = ""; 
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
        SymbolTableEntry *temp;
        SymbolTableEntry *new_entry;
        Variable *new_var;

        temp = lookupScope(yylval.strVal, scope+1); 

        if(temp == NULL){
            new_entry = (SymbolTableEntry*)malloc(sizeof(SymbolTableEntry));
            new_var = (Variable*)malloc(sizeof(Variable));

            temp_func -> arguments[arg_index] = yylval.strVal;
            arg_index++;

            new_var -> name = yylval.strVal;
            new_var -> scope = scope + 1;
            new_var -> line = yylineno;

            new_entry -> isActive = 1;
            new_entry -> varVal = new_var;
            new_entry -> funcVal = NULL;
            new_entry -> type = FORMAL;

            insertEntry(new_entry);
           
        }else{
            
            if(comparelibfunc(yylval.strVal) == -1){
                return;
            }

            int j, flag = 1;
            for(j = 0; j < arg_index; j++){
                flag = strcmp(getEntryName(temp), temp_func -> arguments[j]);
                if(flag == 0) break;
            }
            
            if(temp -> type == FORMAL && flag != 0){
            new_entry = (SymbolTableEntry*)malloc(sizeof(SymbolTableEntry));
            new_var = (Variable*)malloc(sizeof(Variable));

            temp_func -> arguments[arg_index] = yylval.strVal;
            arg_index++;

            new_var -> name = yylval.strVal;
            new_var -> scope = scope + 1;
            new_var -> line = yylineno;

            new_entry -> isActive = 1;
            new_entry -> varVal = new_var;
            new_entry -> funcVal = NULL;
            new_entry -> type = FORMAL;

            insertEntry(new_entry);
            }
            else{
                yyerror("ERROR: Symbol with this name already exists!\t");
            }      
        }
    }
    |ID {
        SymbolTableEntry *temp;
        SymbolTableEntry *new_entry;
        Variable *new_var;

        temp = lookupScope(yylval.strVal, scope+1); 
        
        if(temp == NULL){
            new_entry = (SymbolTableEntry*)malloc(sizeof(SymbolTableEntry));
            new_var = (Variable*)malloc(sizeof(Variable));

            temp_func -> arguments[arg_index] = yylval.strVal;
            arg_index++;

            new_var -> name = yylval.strVal;
            new_var -> scope = scope + 1;
            new_var -> line = yylineno;

            new_entry -> isActive = 1;
            new_entry -> varVal = new_var;
            new_entry -> funcVal = NULL;
            new_entry -> type = FORMAL;

            insertEntry(new_entry);
           
        }else{
            
            if(comparelibfunc(yylval.strVal) == -1){
                return;
            }

            int flag = strcmp(getEntryName(temp), yylval.strVal);
            if(temp -> type == FORMAL && flag != 0){
                new_entry = (SymbolTableEntry*)malloc(sizeof(SymbolTableEntry));
                new_var = (Variable*)malloc(sizeof(Variable));

                temp_func -> arguments[arg_index] = yylval.strVal;
                arg_index++;

                new_var -> name = yylval.strVal;
                new_var -> scope = scope + 1;
                new_var -> line = yylineno;

                new_entry -> isActive = 1;
                new_entry -> varVal = new_var;
                new_entry -> funcVal = NULL;
                new_entry -> type = FORMAL;

                insertEntry(new_entry);
            }
            else{
                yyerror("ERROR: Symbol with this name already exists!\t");
                return;
            }

                 
        }
    }
    COMMA idlist
    |{printf("empty -> idlist\n");}
    ;

ifstmt: IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt   {printf("if(expr) -> ifstmt\n");}
    |IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt ELSE stmt    {printf("if(expr) else -> ifstmt\n");}
    ;

whilestmt: WHILE LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt   {printf("while(expr) -> whilestmt\n");}
    ;    

forstmt: FOR LEFT_PARENTHESIS elist SEMICOLON expr SEMICOLON elist RIGHT_PARENTHESIS stmt
        {printf("for(elist;expr;elist)stmt -> forstmt\n");}
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