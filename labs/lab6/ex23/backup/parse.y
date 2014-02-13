%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "configfile.h"

int yyerror(char *);
int yylex(void);

extern configfile *cf;
int cf_maxsections = 20;
int sec_maxoptions;

void init_sec(configsection *sec, char *name);
void init_opt(configoption *opt, char *name, char *value);

configsection *curr_section;
configoption *curr_option;
%}

%union {
    char *string;
    int num;
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
        cf -> numsections = 1;
        curr_section = cf -> sections;
        init_sec(curr_section, "global");
        sec_maxoptions = 20;
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
        ++(cf -> numsections);
        if(cf -> numsections >= cf_maxsections) {
            cf -> sections = realloc(cf -> sections, 2*cf_maxsections*sizeof(configsection));
            cf_maxsections *= 2;
        }

        //numsections does a good job of keeping track what section we're on
        curr_section = &(cf -> sections[cf -> numsections - 1]);
        init_sec(curr_section, $2);
        //set the max number of opts for this section
        sec_maxoptions = 20;
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
        ++(curr_section -> numopts);
        if(curr_section -> numopts >= sec_maxoptions) {
            curr_section -> options = realloc(curr_section -> options, 2*sec_maxoptions*sizeof(configoption));
            sec_maxoptions *= 2;
        }

        curr_option = &(curr_section -> options[curr_section -> numopts - 1]);
        init_opt(curr_option, $1, $3);
    }
    ;

%%

int yyerror(char *msg) {printf("%s\n", msg);}
void init_sec(configsection *sec, char *name) {
    sec -> name = strdup(name);
    sec -> numopts = 0;
    sec -> options = malloc(20*sizeof(configoption));
    return;
}

void init_opt(configoption *opt, char *name, char *value) {
    opt -> name = strdup(name);
    opt -> value = strdup(value);
    return;
}

