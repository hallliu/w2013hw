typedef enum val_type {
    str,
    integer,
    dbl
} val_type;


typedef union value {
    char *str;
    int integer;
    double real;
} value;

struct configoption {
	char *name;
    val_type type;
    value val;
};

typedef struct configoption configoption;

struct configsection {
	char *name;
	unsigned int numopts;
	configoption *options;
};

typedef struct configsection configsection;

struct configfile {
	unsigned int numsections;
	configsection *sections;
};

typedef struct configfile configfile;
