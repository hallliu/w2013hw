#include <stdio.h>
#include <stdlib.h>
#include "configfile.h"

int yylex(void);
int yyparse(void);

void init_file(configfile *cf);
void print_cf(configfile *cf);

configfile *cf;

int main(int argc, char *argv[]) {
    cf = malloc(sizeof(configfile));
    init_file(cf);
    yyparse();
    print_cf(cf);
    exit(0);
}

void init_file(configfile *cf) {
    cf -> numsections = 0;
    cf -> sections = (configsection *) malloc(20*sizeof(configsection));
    return;
}

void print_cf(configfile *cf) {
    for(int i=0; i < cf -> numsections; i++) {
        printf("SECTION %s\n--------------\n", cf -> sections[i].name);
        for(int j=0; j < cf -> sections[i].numopts; j++) {
            printf("%s=%s\n", cf -> sections[i].options[j].name, cf -> sections[i].options[j].value);
        }
        printf("\n");
    }
    return;
}
