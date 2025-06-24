%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "parser.tab.h" 
        
extern int yylineno;   

#define report_lexical_error(msg) \
    fprintf(stderr, "Lex error (line %d): %s\n", yylineno, msg);
%}



%option yylineno noyywrap

%x LINE_COMMENT
%x MULTILINE_COMMENT

%%

[ \t\r\n]+              { }

"//"                    { BEGIN(LINE_COMMENT); }  

"/*"                    { BEGIN(MULTILINE_COMMENT); }


<LINE_COMMENT>{
"\n"        { BEGIN(INITIAL); }
.           {}
}

<MULTILINE_COMMENT>{
"*/"        { BEGIN(INITIAL); }
.|"\n"      {}
}

\"([^\"\n])*\"      {
    yylval.string = strdup(yytext);
    return STRING;
}


"QUERY"                 { return QUERY; }
"RESULT_OF_QUERY"       { return RESULT_OF_QUERY; }
"EXEC"                  { return EXEC; }
"IF"                    { return IF; }
"FOR"                   { return FOR; }
"IN"                    { return IN; }
"BEGIN"                 { return KW_BEGIN; }
"END"                   { return KW_END; }
"EMPTY"                 { return EMPTY; }
"NOT_EMPTY"             { return NOT_EMPTY; }
"URL_EXISTS"            { return URL_EXISTS; }


"++"                    { return UNION; }
"--"                    { return DIFF; }
"**"                    { return INTER; }


"+"                     { return PLUS; }
"-"                     { return MINUS; }
"*"                     { return STAR; }
"|"                     { return PIPE; }


"<"                     { return LT; }
">"                     { return GT; }
"["                     { return LBR; }
"]"                     { return RBR; }
"("                     { return LPR; }
")"                     { return RPR; }
","                     { return COMMA; }
";"                     { return SC; }
"="                     { return EQ; }
":"                     { return COLON; }


[a-zA-Z_][a-zA-Z0-9_]*   {
    yylval.string = strdup(yytext);
    return WORD;
}


.                        { report_lexical_error("Unexpected character"); }

%%

