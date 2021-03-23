%{
    #include <assert.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>
    
    struct AlphaToken{
    
        int line;
        char* type;
        char* value;
        char* special;    

        struct AlphaToken* next;

    };
    
    struct AlphaToken * tokenListHead = NULL;

    
    void alphaListInsert(int line, char* value, char* type, char* special){
        struct AlphaToken* newToken, *tmp;

        assert(value != NULL);
        assert(type != NULL);


        newToken = (struct AlphaToken *) malloc(sizeof(struct AlphaToken));
        newToken -> line = line;
        newToken -> value = strdup(value);
        newToken -> type = type;
        newToken -> special = special;
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
            printf("%d:  #%d   \"%s\"  %s   %s\n", tmp->line, tokenCount, tmp -> value, tmp-> type, tmp->special);
            tokenCount++;
            tmp = tmp -> next;
        }

        return;
    }



      void commentCheck(int line,char *text){
    }
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
AND             "and"
NOT             "not"
OR              "or"
LOCAL           "local"
TRUE            "true"
FALSE           "false"
NIL             "nil"
OPERATOR_PLUS   "+"
OPERATOR_MINUS  "-"
OPERATOR_MUL    "*"
OPERATOR_DIV    "/"
OPERATOR_ASSIGN "="
OPERATOR_MOD    "%"
OPERATOR_EQ     "=="
OPERATOR_NEQ    "!="
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

MULTI_LINE_COMMENT  "/*"
SINGLE_COMMENT      "//".*
UNKNOWN_TOKEN       .
%%

{MULTI_LINE_COMMENT}    {
                            int commentCloseFlag = 1;
                            int index = 2;
                            int line = yylineno;
                            char charIndex;
                            char* fixedToken = (char *)malloc(1024*sizeof(char*));

                            *(fixedToken + 0) = '/';
                            *(fixedToken + 1) = '*';

                            charIndex = input();
                            while(charIndex != EOF && commentCloseFlag != 0){

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
                                        index++;
                                        charIndex = input();
                                        continue;
                                    }
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
                                        }else{
                                            continue;
                                        }
                                    }
                                }

                                *(fixedToken + index) = charIndex;

                                charIndex = input();
                                index++;        
                            }

                            if(commentCloseFlag != 0){
                                alphaListInsert(line, "ERROR", "Comment never closed", "");
                                return;
                            }
                            alphaListInsert(line, fixedToken, "MULTI_LINE_COMMENT" , "<-enumerated");
                            
}

{SINGLE_COMMENT}      {alphaListInsert(yylineno, yytext, "SINGLE_LINE_COMMENT", "<-enumerated");}
{LEFT_BRACE}          {alphaListInsert(yylineno,yytext,"LEFT_BRACE", "<-enumerated");}
{RIGHT_BRACE}         {alphaListInsert(yylineno,yytext,"RIGHT_BRACE", "<-enumerated");}
{SEMICOLON}           {alphaListInsert(yylineno,yytext,"SEMICOLON", "<-enumerated");}
{COMMA}               {alphaListInsert(yylineno,yytext,"COMMA", "<-enumerated");}
{COLON}               {alphaListInsert(yylineno,yytext,"COLON", "<-enumerated");}
{DOUBLE_COLON}        {alphaListInsert(yylineno,yytext,"DOUBLE_COLON", "<-enumerated");}
{DOT}                 {alphaListInsert(yylineno,yytext,"DOT", "<-enumerated");}
{DOUBLE_DOT}          {alphaListInsert(yylineno,yytext,"DOUBLE_COLON", "<-enumerated");}
{IF}                  {alphaListInsert(yylineno, yytext, "IF_KEYWORD", "<-enumerated");}
{ELSE}                {alphaListInsert(yylineno, yytext, "ELSE_KEYWORD", "<-enumerated");}
{WHILE}               {alphaListInsert(yylineno, yytext, "WHILE_KEYWORD", "<-enumerated");}
{FOR}                 {alphaListInsert(yylineno, yytext, "FOR_KEYWORD", "<-enumerated");}
{FUNCTION}            {alphaListInsert(yylineno, yytext, "FUNCTION_KEYWORD", "<-enumerated");}
{RETURN}              {alphaListInsert(yylineno, yytext, "RETURN_KEYWORD", "<-enumerated");}
{BREAK}               {alphaListInsert(yylineno, yytext, "BREAK_KEYWORD", "<-enumerated");}
{CONTINUE}            {alphaListInsert(yylineno, yytext, "CONTINUE_KEYWORD", "<-enumerated");}
{AND}                 {alphaListInsert(yylineno, yytext, "AND_KEYWORD", "<-enumerated");}
{NOT}                 {alphaListInsert(yylineno, yytext, "NOT_KEYWORD", "<-enumerated");}
{OR}                  {alphaListInsert(yylineno, yytext, "OR_KEYWORD", "<-enumerated");}
{LOCAL}               {alphaListInsert(yylineno, yytext, "LOCAL_KEYWORD", "<-enumerated");}
{TRUE}                {alphaListInsert(yylineno, yytext, "TRUE_KEYWORD", "<-enumerated");}
{FALSE}               {alphaListInsert(yylineno, yytext, "FALSE_KEYWORD", "<-enumerated");}
{NIL}                 {alphaListInsert(yylineno, yytext, "NIL_KEYWORD", "<-enumerated");}
{WHITE_SPACE}         {}
{NEWLINE}             {}
{OPERATOR_PLUS}       {alphaListInsert(yylineno,yytext,"PLUS", "<-enumerated");}
{OPERATOR_MINUS}      {alphaListInsert(yylineno,yytext,"MINUS", "<-enumerated");}
{OPERATOR_MUL}        {alphaListInsert(yylineno,yytext,"MULTIPLICATION", "<-enumerated");}
{OPERATOR_DIV}        {alphaListInsert(yylineno,yytext,"DIVISION", "<-enumerated");}
{OPERATOR_ASSIGN}     {alphaListInsert(yylineno,yytext,"ASSIGNMENT", "<-enumerated");}
{OPERATOR_MOD}        {alphaListInsert(yylineno,yytext,"MODULO", "<-enumerated");}
{OPERATOR_EQ}         {alphaListInsert(yylineno,yytext,"EQUAL", "<-enumerated");}
{OPERATOR_NEQ}        {alphaListInsert(yylineno,yytext,"NEQ", "<-enumerated");}
{OPERATOR_PP}         {alphaListInsert(yylineno,yytext,"INCREMENT", "<-enumerated");}
{OPERATOR_MM}         {alphaListInsert(yylineno,yytext,"DECREMENT", "<-enumerated");}
{OPERATOR_GRT}        {alphaListInsert(yylineno,yytext,"GREATER", "<-enumerated");}
{OPERATOR_LES}        {alphaListInsert(yylineno,yytext,"LESSER", "<-enumerated");}
{OPERATOR_GRE}        {alphaListInsert(yylineno,yytext,"GREATER EQUAL", "<-enumerated");}
{OPERATOR_LEE}        {alphaListInsert(yylineno,yytext,"LESSER EQUAL", "<-enumerated");}
{INTEGER}             {alphaListInsert(yylineno,yytext,"INT", "<-integer");}
{REAL}                {alphaListInsert(yylineno,yytext,"REAL", "<-real");}
{LEFT_BRACKET}        {alphaListInsert(yylineno,yytext,"LEFT_BRACKET", "<-enumerated");}
{RIGHT_BRACKET}       {alphaListInsert(yylineno,yytext,"RIGHT_BRACKET", "<-enumerated");}
{LEFT_PARENTHESIS}    {alphaListInsert(yylineno,yytext,"LEFT_PARENTHESIS", "<-enumerated");}
{RIGHT_PARENTHESIS}   {alphaListInsert(yylineno,yytext,"RIGHT_PARENTHESIS", "<-enumerated");}

{STRING}              {
            char charIndex = '\"';
            int index = 0;
            int backSlashCounter = 0;
            int line = yylineno;
            int closingQuoteFlag = 1;
            char * fixedString =(char*) malloc(1024*sizeof(char*));

            *(fixedString + index) = charIndex;
            index++;

            charIndex = input();
            while(charIndex != EOF){

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

            if(closingQuoteFlag == 0){
                alphaListInsert(line, fixedString, "STRING" , "<-char*");
            }
            else{
                printf("\033[31mIn line %d ERROR: Closing \" expected\033[0m\n",line);
            }

}

{ID}    {alphaListInsert(yylineno, yytext, "ID", "<-char*");}

{UNKNOWN_TOKEN} {printf("\033[31mIn line %d ERROR: Unknown token %s\033[0m\n",line, yytext);}

%%

int main(int argc, char* argv[]){
    
    if(argc < 2){
        printf("No input file!\n");
        return -1;
    }

    if(!(yyin = fopen(argv[1], "r"))){
        printf("Cannot read file!\n");
        return -1;
    }

    yylex();
    printAlphaList();

    return 0;    
}