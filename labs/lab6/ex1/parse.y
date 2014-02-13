%{
#include <stdio.h>
#include <stdlib.h>

int yyerror(char *);
int yylex(void);

%}

%union {
    char *string;
}

%token ERROR OPEN_BRACE CLOSE_BRACE GLOBAL HOST SEMICOLON EQUALS EOFTOK
%token<string> HOSTNAME
%token<string> OPTID
%token<string> OPTVAL

%start input

%%

input
    : global hosts EOFTOK     {YYACCEPT;}
    ;

global
    : gl_open block SEMICOLON
    ;

gl_open
    : GLOBAL {
        printf("GLOBAL CONFIG\n-------------------\n");
    }
    ;
hosts
    : 
    | hosts host
    ;

host
    : h_open block SEMICOLON
    ;

h_open
    : HOST HOSTNAME {
        printf("\nHOST '%s' CONFIG\n-------------------\n", $2);
    }
    ;

block
    : OPEN_BRACE optlist CLOSE_BRACE 
    ;

optlist
    :
    | optlist opt
    ;

opt
    : OPTID EQUALS OPTVAL {
        printf("%s=%s\n", $1, $3);
    }
    ;

%%

int yyerror(char *msg) {printf("%s\n", msg);}
