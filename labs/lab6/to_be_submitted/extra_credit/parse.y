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
void init_opt(configoption *opt, char *name, value val, val_type ty);
void increment_opts();

configsection *curr_section;
configoption *curr_option;
//yes, I know that there are nasty global variables. It's because the structures given didn't
//play too well with resizing arrays.
%}

%union {
    char *string;
    int num;
    double fl;
}

%token ERROR OPEN_BRACE CLOSE_BRACE GLOBAL HOST SEMICOLON EQUALS EOFTOK
%token<string> HOSTNAME
%token<string> OPTID
%token<string> OPTSTRVAL
%token<num> OPTINTVAL
%token<fl> OPTDBLVAL
%token<string> OPTSUB

%type<string> opt_r

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
    : OPTID EQUALS opt_r {
        increment_opts();
        curr_option = &(curr_section -> options[curr_section -> numopts - 1]);
        value v; v.str = $3;
        init_opt(curr_option, $1, v, str);
    }
    | OPTID EQUALS OPTINTVAL {
        increment_opts();
        curr_option = &(curr_section -> options[curr_section -> numopts - 1]);
        value v; v.integer = $3;
        init_opt(curr_option, $1, v, integer);
    }
    | OPTID EQUALS OPTDBLVAL {
        increment_opts();
        curr_option = &(curr_section -> options[curr_section -> numopts - 1]);
        value v; v.real = $3;
        init_opt(curr_option, $1, v, dbl);
    }
    ;

opt_r
    :   {$$ = calloc(1,sizeof(char)); /*This is going to be a null string*/}
    | opt_r OPTSTRVAL {
        //allocate a new string, strcat both old strings in, and free the one we know is dyn. allocated
        char *newstr = malloc((strlen($1)+strlen($2)+1)*sizeof(char));
        strcat(newstr, $1);
        strcat(newstr, $2);
        free($1);
        $$ = newstr;
    }
    | opt_r OPTSUB {
        char *substituted_str = NULL;
        //loop through and search for the right one
        for(int i=0; i < curr_section -> numopts; i++) {
            if(!strcmp($2, curr_section -> options[i].name)) 
                substituted_str = curr_section -> options[i].val.str;
        }
        if(substituted_str == NULL) {
            printf("substitution error\n");
            substituted_str = "";
        }

        char *newstr = malloc((strlen($1)+strlen(substituted_str)+1)*sizeof(char));
        strcat(newstr, $1);
        strcat(newstr, substituted_str);
        free($1);
        $$ = newstr;
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

void increment_opts(void) {
    ++(curr_section -> numopts);
    if(curr_section -> numopts >= sec_maxoptions) {
        curr_section -> options = realloc(curr_section -> options, 2*sec_maxoptions*sizeof(configoption));
        sec_maxoptions *= 2;
    }
    return;
}


void init_opt(configoption *opt, char *name, value val, val_type ty) {
    opt -> name = strdup(name);
    opt -> type = ty;
    if(ty == str) {
        opt -> val.str = strdup(val.str);
    }
    else
        opt -> val = val;
    return;
}

