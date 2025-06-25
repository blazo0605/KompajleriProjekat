#include "ast.h"

Node *astRoot = NULL;

static Node *alloc_node(NodeType t) {
    Node *n = malloc(sizeof(Node));
    if (!n) { perror("malloc"); exit(1); }
    n->type = t;
    n->next = NULL;
    return n;
}

void append_node(Node **list, Node *node) {
    if (!*list) {
        *list = node;
    } else {
        Node *p = *list;
        while (p->next) p = p->next;
        p->next = node;
    }
}



Node *create_program(Node *decls, Node *cmds) {
    Node *n = alloc_node(N_PROGRAM);
    n->program.declarations = decls;
    n->program.commands     = cmds;
    return n;
}

Node *create_query_decl(char *name, Node *query) {
    Node *n = alloc_node(N_QUERY_DECL);
    n->query_decl.name  = strdup(name);
    n->query_decl.query = query;
    return n;
}

Node *create_query_list_decl(char *name, Node *qlist) {
    Node *n = alloc_node(N_QUERY_LIST_DECL);
    n->query_list_decl.name       = strdup(name);
    n->query_list_decl.query_list = qlist;
    return n;
}

Node *create_result_decl(char *name) {
    Node *n = alloc_node(N_RESULT_DECL);
    n->result_decl.name = strdup(name);
    return n;
}

Node *create_exec_command(char *qname) {
    Node *n = alloc_node(N_EXEC_COMMAND);
    n->exec_command.query_name = strdup(qname);
    return n;
}

Node *create_if_command(Node *cond, Node *body) {
    Node *n = alloc_node(N_IF_COMMAND);
    n->if_command.condition = cond;
    n->if_command.body      = body;
    return n;
}

Node *create_for_command(char *it, Node *iterable, Node *body) {
    Node *n = alloc_node(N_FOR_COMMAND);
    n->for_command.iterator = strdup(it);
    n->for_command.iterable = iterable;
    n->for_command.body     = body;
    return n;
}

Node *create_assign_command(char *target, Node *value) {
    Node *n = alloc_node(N_ASSIGN_COMMAND);
    n->assign_command.target = strdup(target);
    n->assign_command.value  = value;
    return n;
}

Node *create_query(Node *expr) {
    Node *n = alloc_node(N_QUERY);
    n->query.expr = expr;
    return n;
}

Node *create_or(Node *l, Node *r) {
    Node *n = alloc_node(N_OR);
    n->binary.left  = l;
    n->binary.right = r;
    return n;
}

Node *create_juxtaposition(Node *l, Node *r) {
    Node *n = alloc_node(N_JUXTAPOSITION);
    n->binary.left  = l;
    n->binary.right = r;
    return n;
}

Node *create_unary(NodeType op, Node *child) {
    Node *n = alloc_node(op);
    n->unary.child = child;
    return n;
}


Node *create_union_op(Node *l, Node *r) {
    Node *n = alloc_node(N_UNION_OP);
    n->binary.left  = l;
    n->binary.right = r;
    return n;
}

Node *create_diff_op(Node *l, Node *r) {
    Node *n = alloc_node(N_DIFF_OP);
    n->binary.left  = l;
    n->binary.right = r;
    return n;
}

Node *create_inter_op(Node *l, Node *r) {
    Node *n = alloc_node(N_INTER_OP);
    n->binary.left  = l;
    n->binary.right = r;
    return n;
}

Node *create_directive(char *key, char *val) {
    Node *n = alloc_node(N_DIRECTIVE);
    n->directive.key   = strdup(key);
    n->directive.value = strdup(val);
    return n;
}

Node *create_term(char *txt) {
    Node *n = alloc_node(N_TERM);
    n->term.text = strdup(txt);
    return n;
}

Node *create_query_ref(char *name) {
    Node *n = alloc_node(N_QUERY_REF);
    n->query_ref.name = strdup(name);
    return n;
}

Node *create_empty_cond(char *name) {
    Node *n = alloc_node(N_EMPTY_COND);
    n->empty_cond.name = strdup(name);
    return n;
}

Node *create_not_empty_cond(char *name) {
    Node *n = alloc_node(N_NOT_EMPTY_COND);
    n->not_empty_cond.name = strdup(name);
    return n;
}

Node *create_url_exists_cond(char *name, char *url) {
    Node *n = alloc_node(N_URL_EXISTS_COND);
    n->url_exists_cond.name = strdup(name);
    n->url_exists_cond.url  = strdup(url);
    return n;
}




static void print_ast_rec(Node *n, int lvl);
static void print_list(Node *list, int lvl);
static void flatten_and_print(Node *n, int lvl);


static void print_indent(int lvl) {
    for (int i = 0; i < lvl; i++) printf("   ");
}


static void print_list(Node *list, int lvl) {
    for (Node *p = list; p; p = p->next) {
        print_ast_rec(p, lvl);
    }
}


static void print_ast_rec(Node *n, int lvl) {
    if (!n) return;

    switch (n->type) {
    case N_PROGRAM:
        printf("Program\n");
        print_indent(lvl+1); printf("Declarations\n");
        print_list(n->program.declarations, lvl+2);
        print_indent(lvl+1); printf("Commands\n");
        print_list(n->program.commands,    lvl+2);
        return;

    case N_QUERY_DECL:
        print_indent(lvl);
        printf("QueryDeclaration \"%s\"\n", n->query_decl.name);
        print_ast_rec(n->query_decl.query, lvl+1);
        return;

    case N_QUERY_LIST_DECL:
        print_indent(lvl);
        printf("QueryListDeclaration \"%s\"\n", n->query_list_decl.name);
        print_list(n->query_list_decl.query_list, lvl+1);
        return;

    case N_RESULT_DECL:
        print_indent(lvl);
        printf("ResultOfQueryDeclaration \"%s\"\n", n->result_decl.name);
        return;

    case N_EXEC_COMMAND:
        print_indent(lvl);
        printf("ExecCommand (QueryName: \"%s\")\n", n->exec_command.query_name);
        return;

    case N_IF_COMMAND:
        print_indent(lvl);    printf("IfCommand\n");
        print_indent(lvl+1);  printf("Condition\n");
        print_ast_rec(n->if_command.condition, lvl+2);
        print_indent(lvl+1);  printf("Body\n");
        print_list(n->if_command.body, lvl+2);
        return;

    case N_FOR_COMMAND:
        print_indent(lvl);
        printf("ForCommand (iterator: %s)\n", n->for_command.iterator);
        print_indent(lvl+1);  printf("Iterable\n");
        print_list(n->for_command.iterable, lvl+2);
        print_indent(lvl+1);  printf("Body\n");
        print_list(n->for_command.body, lvl+2);
        return;

    case N_ASSIGN_COMMAND:
        print_indent(lvl); printf("AssignCommand\n");
        print_indent(lvl+1);
        printf("Target: \"%s\"\n", n->assign_command.target);
        print_ast_rec(n->assign_command.value, lvl+1);
        return;

    case N_QUERY:
        print_indent(lvl); printf("Query\n");
        print_ast_rec(n->query.expr, lvl+1);
        return;

    case N_OR:
        print_indent(lvl); printf("Or\n");
        print_ast_rec(n->binary.left,  lvl+1);
        print_ast_rec(n->binary.right, lvl+1);
        return;

    case N_JUXTAPOSITION:
        print_indent(lvl); printf("Juxtaposition\n");
        flatten_and_print(n, lvl+1);
        return;

    case N_PLUS:
        print_indent(lvl); printf("Plus\n");
        print_ast_rec(n->unary.child, lvl+1);
        return;

    case N_MINUS:
        print_indent(lvl); printf("Minus\n");
        print_ast_rec(n->unary.child, lvl+1);
        return;

    case N_STAR:
        print_indent(lvl); printf("Star\n");
        print_ast_rec(n->unary.child, lvl+1);
        return;

    case N_UNION_OP:
        print_indent(lvl); printf("SetOperation (union)\n");
        print_ast_rec(n->binary.left,  lvl+1);
        print_ast_rec(n->binary.right, lvl+1);
        return;

    case N_DIFF_OP:
        print_indent(lvl); printf("SetOperation (diff)\n");
        print_ast_rec(n->binary.left,  lvl+1);
        print_ast_rec(n->binary.right, lvl+1);
        return;

    case N_INTER_OP:
        print_indent(lvl); printf("SetOperation (inter)\n");
        print_ast_rec(n->binary.left,  lvl+1);
        print_ast_rec(n->binary.right, lvl+1);
        return;

    case N_DIRECTIVE:
        print_indent(lvl);
        printf("Directive (%s:%s)\n", n->directive.key, n->directive.value);
        return;

    case N_TERM:
        print_indent(lvl);
        printf("Term \"%s\"\n", n->term.text);
        return;

    case N_QUERY_REF:
        print_indent(lvl);
        printf("QueryReference \"%s\"\n", n->query_ref.name);
        return;

    case N_EMPTY_COND:
        print_indent(lvl);
        printf("EMPTY(%s)\n", n->empty_cond.name);
        return;

    case N_NOT_EMPTY_COND:
        print_indent(lvl);
        printf("NOT_EMPTY(%s)\n", n->not_empty_cond.name);
        return;

    case N_URL_EXISTS_COND:
        print_indent(lvl);
        printf("URL_EXISTS(%s, \"%s\")\n",
               n->url_exists_cond.name, n->url_exists_cond.url);
        return;
    }
}


static void flatten_and_print(Node *n, int lvl) {
    if (n->type == N_JUXTAPOSITION) {
        flatten_and_print(n->binary.left, lvl);
        flatten_and_print(n->binary.right, lvl);
    } else {
        print_ast_rec(n, lvl);
    }
}


void print_ast(Node *root) {
    print_ast_rec(root, 0);
}