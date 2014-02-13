%{
#include <stdio.h>
#include <stdlib.h>
#include "parse.tab.h"

int yyparse();
%}

%option noyywrap

%x GLOBAL_ST HOST_ST BLOCK OPT

acceptable_str [[:alpha:]]([[:alnum:]]|_)*
optval ("-"?[[:digit:]]*"."*[[:digit:]]*)|(([[:alpha:]]|"/")[[:alnum:]_\-./]*)

substval %{acceptable_str}%
%%

    int originating_state;

<*>[ \t]   {}

<INITIAL>global {
    originating_state = GLOBAL_ST; 
    BEGIN GLOBAL_ST; 
    return GLOBAL;
}
<INITIAL>host {
    originating_state = HOST_ST;
    BEGIN HOST_ST; 
    return HOST;
}

<GLOBAL_ST,HOST_ST>\{  {BEGIN BLOCK; return OPEN_BRACE;}
<BLOCK>\}  {BEGIN originating_state; return CLOSE_BRACE;}

<HOST_ST>"\""{optval}"\"" {
    //strip off the quotation marks
    int i = strlen(yytext);
    yytext[i-1] = 0;
    yylval.string = strdup(yytext + 1);
    return HOSTNAME;
}

<BLOCK>{acceptable_str} {
    yylval.string = strdup(yytext);
    BEGIN OPT;
    return OPTID;
}

<OPT>= {return EQUALS;}
<OPT>{substval} {
    int i = strlen(yytext);
    yytext[i-1]=0;
    yylval.string = strdup(yytext + 1);
    return OPTSUB;
}
<OPT>{optval} {
    yylval.string = strdup(yytext);
    return OPTVAL;
}
<OPT>\n {BEGIN BLOCK;}

<*>; {BEGIN INITIAL; return SEMICOLON;}

<*>. {return ERROR;}
<*>\n {}
<<EOF>> {return EOFTOK;}
%%
