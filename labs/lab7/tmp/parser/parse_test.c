#include <stdio.h>
#include <stdlib.h>
#include "lsystem.h"
#include "parse.tab.h"

int yyparse(void);
L_Lsystem *lsystem;

int main(int argc, char *argv[]) {
    L_rules *r;
    L_subs *s;
    lsystem = malloc(sizeof(L_Lsystem));
    yyparse();
    printf("limit: %d\nangle: %.1f\nstart: %c\n", lsystem -> limit, lsystem -> angle, lsystem -> start);
    printf("----------\n");
    r = lsystem -> rules;
    s = lsystem -> subs;
    while(r) {
        printf("From %c to %s\n", r -> lhs, r -> rhs);
        r = r -> nextrule;
    }
    printf("-----------\n");
    while(s) {
        printf("From %c to %s\n", s -> lhs, s -> rhs);
        s = s -> nextsub;
    }
    exit(0);
}
