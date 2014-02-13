CFLAGS = -std=gnu99 -ggdb -Wall -pedantic
LEX = flex

lm2tg: lm2tg.o parse.tab.o token.o crawl.o

parse_test: parse_test.o parse.tab.o token.o

parse_test.o: parse_test.c lsystem.h parse.tab.h

lm2tg.o : parse.tab.h lm2tg.c

parse.tab.c parse.tab.h: parse.y
	bison -d parse.y

parse.tab.o: parse.tab.c lsystem.h

token.c: token.l
token.o: token.c parse.tab.h

clean:
	rm -f *.o parse_test parse.tab.[ch] token.c lm2tg
