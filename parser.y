%{
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

extern int yylineno;
extern Node *astRoot;

void yyerror(const char *s);
int yylex(void);


static Node *append(Node *list, Node *item) {
    append_node(&list, item);
    return list;
}
%}

%code requires {
#include "ast.h"
}

%union {
    Node *node;
    char *string;
}

%token <string> WORD STRING

%token QUERY RESULT_OF_QUERY EXEC IF FOR IN KW_BEGIN KW_END
%token EMPTY NOT_EMPTY URL_EXISTS
%token UNION DIFF INTER PLUS MINUS STAR PIPE
%token LT GT LBR RBR LPR RPR COMMA SC EQ COLON

%type <node> program declarations declaration commands command
%type <node> assign_command condition list_of_queries list_items list_item
%type <node> query or_expr and_terms term directive

%start program

%%

program
    : declarations commands
      {
        astRoot = create_program($1, $2);
        print_ast(astRoot);
      }
    ;

declarations
    : 
        { $$ = NULL; }
    | declarations declaration
        { $$ = append($1, $2); }
    ;

declaration
    : QUERY WORD EQ query SC
        { $$ = create_query_decl($2, $4); }
    | QUERY WORD EQ list_of_queries SC
        { $$ = create_query_list_decl($2, $4); }
    | RESULT_OF_QUERY WORD SC
        { $$ = create_result_decl($2); }
    ;

commands
    : 
        { $$ = NULL; }
    | commands command
        { $$ = append($1, $2); }
    ;

command
    : EXEC WORD SC
        { $$ = create_exec_command($2); }
    | IF condition KW_BEGIN commands KW_END
        { $$ = create_if_command($2, $4); }
    | FOR WORD IN list_of_queries KW_BEGIN commands KW_END
        { $$ = create_for_command($2, $4, $6); }
    | assign_command SC
        { $$ = $1; }
    ;

assign_command
    : WORD EQ EXEC WORD
        { $$ = create_assign_command($1,
                    (Node*)create_exec_command($4)); }
    | WORD EQ WORD UNION WORD
        { $$ = create_assign_command($1,
                    create_union_op(create_query_ref($3), create_query_ref($5))); }
    | WORD EQ WORD DIFF WORD
        { $$ = create_assign_command($1,
                    create_diff_op(create_query_ref($3), create_query_ref($5))); }
    | WORD EQ WORD INTER WORD
        { $$ = create_assign_command($1,
                    create_inter_op(create_query_ref($3), create_query_ref($5))); }
    ;

condition
    : EMPTY LPR WORD RPR
        { $$ = create_empty_cond($3); }
    | NOT_EMPTY LPR WORD RPR
        { $$ = create_not_empty_cond($3); }
    | URL_EXISTS LPR WORD COMMA STRING RPR
        { $$ = create_url_exists_cond($3, $5); }
    ;

list_of_queries
    : LBR list_items RBR
        { $$ = $2; }
    ;

list_items
    : list_item
        { $$ = $1; }
    | list_items COMMA list_item
        { $$ = append($1, $3); }
    ;

list_item
    : query
        { $$ = $1; }
    | WORD
        { $$ = create_query_ref($1); }
    ;

query
    : LT or_expr GT
        { $$ = create_query($2); }
    ;

or_expr
    : and_terms
        { $$ = $1; }
    | or_expr PIPE and_terms
        { $$ = create_or($1, $3); }
    ;

and_terms
    : term
        { $$ = $1; }
    | and_terms term
        { $$ = create_juxtaposition($1, $2); }
    ;

term
    : WORD       { $$ = create_term($1); }
    | STRING     { $$ = create_term($1); }
    | directive  { $$ = $1; }
    | PLUS term  { $$ = create_unary(N_PLUS, $2); }
    | MINUS term { $$ = create_unary(N_MINUS, $2); }
    | STAR term  { $$ = create_unary(N_STAR, $2); }
    | LPR or_expr RPR {$$ = $2}
    ;

directive
    : WORD COLON WORD
        { $$ = create_directive($1, $3); }
    | WORD COLON STRING
        { $$ = create_directive($1, $3); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error (line %d): %s\n", yylineno, s);
}

int main(void) {
    if (yyparse() == 0) {
        
    }
    return 0;
}
