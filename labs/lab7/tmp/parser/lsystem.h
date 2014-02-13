/*Watson Ladd watsonbladd@gmail.com *
 *Hall Liu hallliu000@gmail.com*
 *Grant Rotskoff gmr1887@gmail.com */
typedef struct L_Lsystem{
  int limit;
  double angle;
  char start;
  struct L_rules * rules;
  struct L_subs *subs;
} L_Lsystem;

typedef struct L_rules {
  struct L_rules *nextrule;
  char lhs;
  char *rhs;
} L_rules;


typedef struct L_subs {
  struct L_subs *nextsub;
  char lhs;
  char *rhs;
} L_subs;

extern L_Lsystem *lsystem;
extern char *rules[256];
extern char *substitutions[256];
