CFLAGS = -std=gnu99
LEX = flex

configparse: configparse.o tokens.o parse.tab.o

configparse.o: configparse.c configfile.h

parse.tab.c parse.tab.h: parse.y
	bison -d parse.y

parse.tab.o: parse.tab.c configfile.h

tokens.o: tokens.c parse.tab.h
tokens.c: tokens.l

clean: 
	rm -f *.o tokens.c parse.tab.[ch] configparse

