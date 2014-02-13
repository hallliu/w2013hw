CFLAGS = -std=gnu99
LEX = flex

configparse: tokens.o parse.tab.o

parse.tab.c parse.tab.h: parse.y
	bison -d parse.y

tokens.o: tokens.c parse.tab.h
tokens.c: tokens.l

clean: 
	rm -f *.o tokens.c parse.tab.[ch] configparse
