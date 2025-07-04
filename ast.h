#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    N_PROGRAM,

    
    N_QUERY_DECL,
    N_QUERY_LIST_DECL,
    N_RESULT_DECL,

    
    N_EXEC_COMMAND,
    N_IF_COMMAND,
    N_FOR_COMMAND,
    N_ASSIGN_COMMAND,

    
    N_QUERY,
    N_OR,               
    N_JUXTAPOSITION,    

    
    N_PLUS,
    N_MINUS,
    N_STAR,

   
    N_UNION_OP,         
    N_DIFF_OP,          
    N_INTER_OP,         

    N_DIRECTIVE,
    N_TERM,
    N_QUERY_REF,

    
    N_EMPTY_COND,
    N_NOT_EMPTY_COND,
    N_URL_EXISTS_COND
} NodeType;

typedef struct Node {
    NodeType type;
    struct Node *next;  

    union {
        
        struct {
            struct Node *declarations;
            struct Node *commands;
        } program;

        
        struct {
            char *name;
            struct Node *query;
        } query_decl;
        struct {
            char *name;
            struct Node *query_list;
        } query_list_decl;
        struct {
            char *name;
        } result_decl;

        
        struct {
            char *query_name;
        } exec_command;
        struct {
            struct Node *condition;
            struct Node *body;
        } if_command;
        struct {
            char *iterator;
            struct Node *iterable;
            struct Node *body;
        } for_command;
        struct {
            char *target;
            struct Node *value;
        } assign_command;

        
        struct {
            struct Node *expr;
        } query;
        struct {
            struct Node *left, *right;
        } binary;      
        struct {
            struct Node *child;
        } unary;      
        struct {
            char *key, *value;
        } directive;
        struct {
            char *text;
        } term;
        struct {
            char *name;
        } query_ref;

        
        struct {
            char *name;
        } empty_cond;
        struct {
            char *name;
        } not_empty_cond;
        struct {
            char *name;
            char *url;
        } url_exists_cond;
    };
} Node;



Node *create_program(Node *decls, Node *cmds);

Node *create_query_decl(char *name, Node *query);
Node *create_query_list_decl(char *name, Node *qlist);
Node *create_result_decl(char *name);

Node *create_exec_command(char *qname);
Node *create_if_command(Node *cond, Node *body);
Node *create_for_command(char *it, Node *iterable, Node *body);
Node *create_assign_command(char *target, Node *value);

Node *create_query(Node *expr);
Node *create_or(Node *l, Node *r);
Node *create_juxtaposition(Node *l, Node *r);

Node *create_unary(NodeType op, Node *child);


Node *create_union_op(Node *l, Node *r);
Node *create_diff_op(Node *l, Node *r);
Node *create_inter_op(Node *l, Node *r);

Node *create_directive(char *key, char *val);
Node *create_term(char *txt);
Node *create_query_ref(char *name);

Node *create_empty_cond(char *name);
Node *create_not_empty_cond(char *name);
Node *create_url_exists_cond(char *name, char *url);



void append_node(Node **list, Node *node);
void print_ast(Node *root);

#endif 
