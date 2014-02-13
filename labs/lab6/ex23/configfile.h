struct configoption {
	char *name;
	char *value;
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
