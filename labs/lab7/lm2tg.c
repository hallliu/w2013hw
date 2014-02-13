// lm2tg.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lsystem.h"
#include "parse.tab.h"

FILE *yyin;
char *rules[256];
char *substitutions[256];
L_Lsystem *lsystem;
int yyparse();

void crawl(L_Lsystem *system){
  L_rules *np;
  for(np=system->rules; np; np=np->nextrule){
    rules[np->lhs]=np->rhs;
  }
  L_subs *ns;
  for(ns=system->subs; ns; ns=ns->nextsub){
    substitutions[ns->lhs]=ns->rhs;
  }
}


int generateoutput(L_Lsystem *system, char *outfile) {
	FILE *fp = fopen(outfile,"w");
	// populate the rule/substitution arrays
	crawl(system);
	//initial rule
	char *tgstring = malloc(2);
	tgstring[0] = system->start;
	tgstring[1] = 0;
	
	int n=1;
	
	//Build the string to be translated into instructions
	int len=1;
	int loc=0;
	char *result = malloc(1);
	while (n < system->limit) {
		
		int i = 0;
		while (tgstring[i] != 0) {
			if (rules[tgstring[i]] == 0) {
				if (loc < len + 1) {
					// insufficient space
					realloc(result,sizeof(result)+1);
					len++;
				}
				result[loc++] = tgstring[i];
			}
			else {
				if (loc + strlen(rules[tgstring[i]]) < len) {
					realloc(result,2*sizeof(result));
					len = 2*len;
				}
				result[loc] = tgstring[i];
				loc = loc + strlen(rules[tgstring[i]]);
			}
		}
		n++;
	}
	
	//Substitutions
	for (n=0;n<len;n++) {
		fprintf(fp,"%s\n",substitutions[result[n]]);
	}
	fclose(fp);
}


int main(int argc,char **argv) {
	// define yyin() so that we can read from a file
	lsystem = malloc(sizeof(L_Lsystem));
	yyin = fopen(argv[1],"r");
	yyparse();
	generateoutput(lsystem,strcat(argv[1],".out"));
}
	
	
