#include "lsystem.h"
#include<stdio.h>
void crawl( L_Lsystem *system){
  for(int i=0; i<256; i++){
    rules[i]=0;
    substitutions[i]=0;
  }
  L_rules *np;
  for(np=system->rules; np; np=np->nextrule){
    rules[np->lhs]=np->rhs;
  }
  L_subs *ns;
  for(ns=system->subs; ns; ns=ns->nextsub){
    asprintf(&substitutions[ns->lhs], "%s",ns->rhs);
  }
  asprintf(&substitutions['+'], "left %f", system->angle);
  asprintf(&substitutions['-'],"right %f", system->angle);
  asprintf(&substitutions['['], "push");
  asprintf(&substitutions[']'], "pop");
}
