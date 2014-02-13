#include <stdio.h>
#include <stdlib.h>

int yylex(void);
int yyparse(void);
void parse_string(char *str);

int main(int argc, char *argv[]) {
    yyparse();
    exit(0);
}
