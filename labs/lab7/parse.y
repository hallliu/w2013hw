%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lsystem.h"

int yyerror(char *msg);
int yylex(void);
//function declarations
void add_rule(L_Lsystem *ls, L_rules *r);
void add_sub(L_Lsystem *ls, L_subs *s);

L_rules *init_rule(char var, char *rhs);
L_subs *init_sub(char var, char *subst);

extern L_Lsystem *lsystem;
extern FILE *yyin;
%}

%union {
    char *string;
    char character;
    int integer;
    double real;
}

%token ERROR EQ ARROW_S ARROW_D EOFTOK
%token<string> PARAM_NAME RULE_R SUBST_R
%token<character> SUBST_L RULE_L PARAM_VARVAL
%token<integer> PARAM_DECVAL
%token<real> PARAM_REALVAL

%start input
%%

input 
    : params rules substitutions EOFTOK {YYACCEPT;}
    ;

params
    : params param
    |
    ;

param
    : PARAM_NAME EQ PARAM_VARVAL {
        //parsing of "invalid" parameters stops here.
        if(!strcmp($1,"start")) {
            lsystem -> start = $3;
        }
    }
    | PARAM_NAME EQ PARAM_DECVAL {
        //only consider the cases that the structs are built to handle
        if(!strcmp($1,"limit")) {
            lsystem -> limit = $3;
        }
        else if(!strcmp($1,"angle")) {
            lsystem -> angle = $3;
        }
    }
    | PARAM_NAME EQ PARAM_REALVAL {
        if(!strcmp($1,"angle")) {
            lsystem -> angle = $3;
        }
    }
    ;
    
rules
    : rules rule
    |
    ;

rule
    : RULE_L ARROW_S RULE_R {
        L_rules *r = init_rule($1, $3);
        add_rule(lsystem, r);
    }
    ;

substitutions
    : substitutions sub
    |
    ;

sub
    : SUBST_L ARROW_D SUBST_R {
        L_subs *s = init_sub($1,$3);
        add_sub(lsystem, s);
    }
    ;

%%

char *break_and_strip(char* str) { //replaces ;s with \ns and strips out some whitespace. nice function name, eh?
    char *result_final = malloc(strlen(str)*sizeof(char)); //we'll never need more space than the original
    char *result = result_final;
    int in_leading_white = 0;
    while(*str) {
        if(*str == ';') {
            *result = '\n';
            ++result;
            in_leading_white = 1;
        }
        else if(*str == ' ' && in_leading_white) {
        }
        else {
            *result = *str;
            ++result;
            in_leading_white = 0;
        }
        ++str;
    }
    *result = '\0';
    return result_final;
}

L_subs *init_sub(char var, char *subst) {
    L_subs *s = malloc(sizeof(L_subs));
    s -> nextsub = NULL;
    s -> lhs = var;
    s -> rhs = break_and_strip(subst);
    return s;
}

void add_sub(L_Lsystem *ls, L_subs *s) {
    s -> nextsub = ls -> subs;
    ls -> subs = s;
    return;
}
L_rules *init_rule(char var, char *rhs) {
    L_rules *r = malloc(sizeof(L_rules));
    r -> nextrule = NULL;
    r -> lhs = var;
    r -> rhs = rhs;
    return r;
}

void add_rule(L_Lsystem *ls, L_rules *r) {
    r -> nextrule = ls -> rules;
    ls -> rules = r;
    return;
}

int yyerror(char *msg) {}
