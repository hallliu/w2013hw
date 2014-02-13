// lm2tg.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "lsystem.h"
#include "parse.tab.h"

extern FILE *yyin;
char *rules[256];
char *substitutions[256];
L_Lsystem *lsystem;
int yyparse();
extern crawl(L_Lsystem *system);

int generateoutput(L_Lsystem *system, FILE *fp) {
	// populate the rule/substitution arrays
	crawl(system);
	//initial rule
	char *tgstring = malloc(2);
	tgstring[0] = system->start;
	tgstring[1] = 0;
	int n=0;
	//Build the string to be translated into instructions
        while (n < system->limit) {
        int len=1;
	int loc=0;
        char c =0;
	char *result = malloc(1);
        *result=0;
          for(int i=0; (c=tgstring[i])!=0; i++){
            assert(result[loc]==0);
            if(rules[c]==NULL){//we have a copy to do
              while(loc > len-2){
                result=realloc(result, 2*len);
                len=2*len;
              }
              result[loc]=c;
              loc++;
              result[loc]=0; //null terminate result
            }else { //
              while(loc+strlen(rules[c])+1>len-1){
                result=realloc(result, 2*len);
                len=2*len;
              }
              strcat(result, rules[c]);
              loc+=strlen(rules[c]);
            }
          }
          tgstring=result;
          n++;
        }
	//Substitutions
	for (n=0;tgstring[n];n++) {
		fprintf(fp,"%s\n",substitutions[tgstring[n]]);
	}
	fclose(fp);
}


int main(int argc,char **argv) {
  if(argc!=2){ fprintf(stderr, "Usage: lm2tg file.lm\n");}
  // define yyin() so that we can read from a file
  lsystem = malloc(sizeof(L_Lsystem));
  yyin = fopen(argv[1],"r");
  yyparse();
  generateoutput(lsystem,stdout);
}
	
	
