#include "lsystem.h"
void crawl( L_Lsystem *system){
  L_rules *np;
  for(np=system->rules; np; np=np->nextrule){
    rules[np->lhs]=np->rhs;
  }
  L_subs *ns;
  for(ns=system->subs; ns; ns=ns->nextsub){
    substitutions[ns->lhs]=ns->rhs;
  }
}
