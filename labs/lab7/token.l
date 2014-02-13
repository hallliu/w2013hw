%{
#include <stdio.h>
#include <stdlib.h>
#include "parse.tab.h"

int yyparse();
%}

%option noyywrap

%x RULE_BL SUBST_BL PARAM RULE SUBST QUOTE

param [[:lower:]]+
integer [[:digit:]]+
real [[:digit:]]+"."[[:digit:]]*

rulestring [A-Z+\-\[\]]+
%%

<INITIAL,RULE_BL,SUBST_BL,PARAM,RULE,SUBST>[ \t] {}
<INITIAL>{param} {
    BEGIN PARAM;
    yylval.string = strdup(yytext);
    return PARAM_NAME;
}

<INITIAL>%% {
    BEGIN RULE_BL;
}

<PARAM>= {
    return EQ;
}
<PARAM>[[:upper:]] {
    yylval.character = yytext[0];
    return PARAM_VARVAL;
}
<PARAM>{integer} {
    yylval.integer = atoi(yytext);
    return PARAM_DECVAL;
}
<PARAM>{real} {
    yylval.real = strtod(yytext, NULL);
    return PARAM_REALVAL;
}
<PARAM>\n {
    BEGIN INITIAL;
}

<RULE_BL>[[:upper:]] {
    yylval.character = yytext[0];
    BEGIN RULE;
    return RULE_L;
}
<RULE>"->" {
    return ARROW_S; //single arrow
}
<RULE>{rulestring} {
    yylval.string = strdup(yytext);
    return RULE_R;
}
<RULE>; {
    BEGIN RULE_BL;
}
<RULE_BL>%% {
    BEGIN SUBST_BL;
}

<SUBST_BL>[[:upper:]] {
    yylval.character = yytext[0];
    BEGIN SUBST;
    return SUBST_L;
}
<SUBST>"=>" {
    return ARROW_D;
}
<SUBST>"\"" {
    BEGIN QUOTE;
}

<QUOTE>[^\"\n]+ { 
    yylval.string = strdup(yytext);
    return SUBST_R;
}
<QUOTE>"\"" {
    BEGIN SUBST;
}

<SUBST>; {
    BEGIN SUBST_BL;
}
<SUBST_BL><<EOF>> {
    return EOFTOK;
}

<*>. {return ERROR;}
<*>\n {}
<*><<EOF>> {return ERROR;}
%%

