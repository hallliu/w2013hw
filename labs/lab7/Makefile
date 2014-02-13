CFLAGS = -std=gnu99
LEX = flex
VPATH = ..

lm2tg: lm2tg.o parse.tab.o token.o

lm2tg.o: lm2tg.c lsystem.h parse.tab.h

parse.tab.c parse.tab.h: parse.y
	bison -d parse.y

parse.tab.o: parse.tab.c lsystem.h

token.c: token.l
token.o: token.c parse.tab.h

clean:
	rm -f *.o parse_test parse.tab.[ch] token.c
