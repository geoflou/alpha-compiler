%{
    #include <assert.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>
    #include "grammar.h"
    #include "SymbolTable.h"

    /*for double/real values we use strtod (str to double)*/
    char* useless_ptr;
    /*paizei na uparxei problima me autin tin sunartisi giati
    pairnei char*, char** kai den 3erw an to yytext einai char*
    (to char** pou pairnei einai gia na balei mesa oti den einai double
    gia na min paei skoupidia i guess)*/

    /*start of useless code*/
    struct AlphaToken{
    
        int line;
        char* type;
        char* value;    

        struct AlphaToken* next;

    };
    
    struct AlphaToken * tokenListHead = NULL;

    
    void alphaListInsert(int line, char* value, char* type){
        struct AlphaToken* newToken, *tmp;

        assert(value != NULL);
        assert(type != NULL);


        newToken = (struct AlphaToken *) malloc(sizeof(struct AlphaToken));
        newToken -> line = line;
        newToken -> value = strdup(value);
        newToken -> type = type;
        newToken -> next = NULL;

        tmp = tokenListHead;

        if(tmp == NULL){
            tokenListHead = newToken;
            return;
        }

        while(tmp -> next != NULL){
        tmp = tmp -> next;
    }

        tmp -> next = newToken;
        return;

    }

    void printAlphaList(){
        struct AlphaToken * tmp = tokenListHead;
        int tokenCount = 1;

        if(tmp == NULL){
            printf("No tokens recognized\n");
            return;
        }

        while(tmp != NULL){
            printf("%d:  #%d   \"%s\"  %s\n", tmp->line, tokenCount, tmp -> value, tmp-> type);
            tokenCount++;
            tmp = tmp -> next;
        }

        return;
    }
    /*end of useless code*/   
    
%}

%option yylineno
%option noyywrap
%x comment
ID              [a-zA-Z][a-zA-Z_0-9]*
WHITE_SPACE           [\r \t]
NEWLINE         \n
STRING          \"
IF              "if"
ELSE            "else"
WHILE           "while"
FOR             "for"
FUNCTION        "function"
RETURN          "return"
BREAK           "break"
CONTINUE        "continue"

LOCAL_KEYWORD           "local"
TRUE            "true"
FALSE           "false"
NIL             "nil"
NOT    "not"
OPERATOR_PLUS   "+"
OPERATOR_MINUS  "-"
OPERATOR_MUL    "*"
OPERATOR_DIV    "/"
OPERATOR_ASSIGN "="
OPERATOR_MOD    "%"
OPERATOR_EQ     "=="
OPERATOR_NEQ    "!="
AND    "and"
OR     "or"
OPERATOR_PP     "++"
OPERATOR_MM     "--"
OPERATOR_GRT    ">"
OPERATOR_LES    "<"
OPERATOR_GRE    ">="
OPERATOR_LEE    "<="
INTEGER         [0-9]{1,}
REAL            [0-9]*\.?[0-9]{1,}
LEFT_BRACKET        "{"
RIGHT_BRACKET       "}"
LEFT_PARENTHESIS    "("
RIGHT_PARENTHESIS   ")"
LEFT_BRACE          "["
RIGHT_BRACE         "]"
SEMICOLON           ";"
COMMA               ","
COLON               ":"
DOUBLE_COLON        "::"
DOT                 "."
DOUBLE_DOT          ".."

MULTI_COMMENT       "/*"
SLASH_COMMENT      "//".*
UNKNOWN_TOKEN       .
%%


{MULTI_COMMENT}    {
                            int commentCloseFlag = 1;
                            int index = 2;
                            int line = yylineno;
                            char charIndex;
                            int size = 1024;
                            char* tmp;
                            char* fixedToken = (char *)malloc(1024*sizeof(char*));

                            *(fixedToken + 0) = '/';
                            *(fixedToken + 1) = '*';

                            charIndex = input();
                            while(charIndex != EOF && charIndex != '\0' && commentCloseFlag != 0){
                                
                                if(index == size) {
                                    tmp = (char*)realloc(fixedToken, size*2*sizeof(char));
                                    size = size * 2;
                                    if(tmp == NULL) {
                                        printf("\033[31mRealloc Failed\033[0m\n");
                                        return -1;
                                    }
                                    fixedToken = tmp;
                                }


                                if(charIndex == '\n'){
                                    charIndex = input();
                                    continue;
                                }


                                if(charIndex == '/'){
                                    *(fixedToken + index) = charIndex;
                                    index++;
                                    charIndex = input();
                                    if(charIndex == '*'){
                                       *(fixedToken + index) = charIndex;
                                        commentCloseFlag++; 
                                        charIndex = input();
                                        index++;
                                    }
                                        continue;
                                }

                                if(charIndex == '*'){
                                    *(fixedToken + index) = charIndex;
                                    index++;
                                    charIndex = input();
                                    if(charIndex == '/'){
                                        *(fixedToken + index) = charIndex;
                                        commentCloseFlag--;
                                        index++;
                                        charIndex = input();
                                        if(commentCloseFlag == 0){
                                            break;
                                        }
                                        continue;
                                        
                                    }
                                }

                                *(fixedToken + index) = charIndex;

                                charIndex = input();
                                index++;     
                            }

                            if(commentCloseFlag != 0){
                                printf("\033[31mIn line %d ERROR: Comment never closed!\033[0m\n",line);
                                return -1;
                            }

                            
}
{SLASH_COMMENT}      {}

{LEFT_BRACE}          {return LEFT_BRACE;}
{RIGHT_BRACE}         {return RIGHT_BRACE;}
{SEMICOLON}           {return SEMICOLON;}
{COMMA}               {return COMMA;}
{COLON}               {return COLON;}
{DOUBLE_COLON}        {return DOUBLE_COLON;}
{DOT}                 {return DOT;}
{DOUBLE_DOT}          {return DOUBLE_DOT;}
{IF}                  {return IF;}
{ELSE}                {return ELSE;}
{WHILE}               {return WHILE;}
{FOR}                 {return FOR;}
{FUNCTION}            {yylval.strVal = strdup(yytext);
                        return FUNCTION;
                        }
{RETURN}              {return RETURN;}
{BREAK}               {return BREAK;}
{CONTINUE}            {return CONTINUE;}

{LOCAL_KEYWORD}       {return LOCAL_KEYWORD;}
{TRUE}                {return TRUE;}
{FALSE}               {return FALSE;}
{NIL}                 {return NIL;}
{WHITE_SPACE}         {}
{NEWLINE}             {}
{NOT}        	      {return NOT;}
{OPERATOR_PLUS}       {return OPERATOR_PLUS;}
{OPERATOR_MINUS}      {return OPERATOR_MINUS;}
{OPERATOR_MUL}        {return OPERATOR_MUL;}
{OPERATOR_DIV}        {return OPERATOR_DIV;}
{OPERATOR_ASSIGN}     {return OPERATOR_ASSIGN;}
{OPERATOR_MOD}        {return OPERATOR_MOD;}
{AND}		          {return AND;}
{OR}		          {return OR;}
{OPERATOR_EQ}         {return OPERATOR_EQ;}
{OPERATOR_PP}         {return OPERATOR_PP;}
{OPERATOR_MM}         {return OPERATOR_MM;}
{OPERATOR_GRT}        {return OPERATOR_GRT;}
{OPERATOR_LES}        {return OPERATOR_LES;}
{OPERATOR_GRE}        {return OPERATOR_GRE;}
{OPERATOR_LEE}        {return OPERATOR_LEE;}
{OPERATOR_NEQ}        {return OPERATOR_NEQ;}
{INTEGER}             {yylval.intVal = atoi(yytext);
                        return INTEGER;}
{REAL}                {yylval.doubleVal = strtod(yytext, &useless_ptr);
                        return REAL;}
{LEFT_BRACKET}        {return LEFT_BRACKET;}
{RIGHT_BRACKET}       {return RIGHT_BRACKET;}
{LEFT_PARENTHESIS}    {return LEFT_PARENTHESIS;}
{RIGHT_PARENTHESIS}   {return RIGHT_PARENTHESIS;}

{STRING}              {
            char charIndex = '\"';
            int index = 0;
            int backSlashCounter = 0;
            int line = yylineno;
            int closingQuoteFlag = 1;
            int size = 1024;
            char* tmp;
            char* fixedString =(char*) malloc(1024*sizeof(char*));

            *(fixedString + index) = charIndex;
            index++;

            charIndex = input();
            while(charIndex != EOF && charIndex != '\0'){
                
                if(index >= size) {
                    tmp = (char*)realloc(fixedString, size*2*sizeof(char));
                    size = size * 2;
                    if(tmp == NULL) {
                        printf("\033[31mRealloc Failed\033[0m\n");
                        return -1;
                    }
                    fixedString = tmp;
                }

                if(charIndex == '\\'){
                    charIndex = input();

                    if(charIndex == 'n'){
                        *(fixedString + index) = '\n';
                        index += 1;
                        charIndex = input();
                        continue;
                    }

                    if(charIndex == 't'){
                        *(fixedString + index) = '\t';
                        index += 1;
                        charIndex = input();
                        continue;
                    }

                    
                    if(charIndex == '\"'){
                        *(fixedString + index) = '\"';
                        index += 1;
                        charIndex = input();
                        continue;
                    }

                    if(charIndex == '\\'){
                        backSlashCounter++;
                        *(fixedString + index) = '\\';
                        index += 1;
                        charIndex = input();
                        backSlashCounter++;
                        continue;
                    }
                    else {
                        printf("\033[31mIn line %d ERROR: Invalid escape sequence\033[0m\n",line);
                        return -1;
                    }
                    
                }


                if(charIndex == '\"'){
                    *(fixedString + index) = charIndex;
                    closingQuoteFlag = 0;
                    break;
                }


                *(fixedString + index) = charIndex;


                index++;
                charIndex = input();
            }

            if(backSlashCounter % 2 != 0){
                printf("\033[31mIn line %d ERROR: Invalid backslash escape sequence\033[0m\n",line);
                return -1;
            }

            if(closingQuoteFlag != 0){
                printf("\033[31mIn line %d ERROR: Closing \" expected\033[0m\n",line);
                return -1;
            }
            
            yylval.strVal = strdup(fixedString);
            return STRING;

}


{ID}    {yylval.strVal = strdup(yytext);
            return ID;}

%%

