#
# makefile for TINY
# Borland C Version
# K. Louden 2/3/98
#

CC = gcc

CFLAGS = -Wall -g 

OBJS = y.tab.o lex.yy.o main.o util.o symtab.o analyze.o code.o cgen.o

cminus: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o cminus -lfl

y.tab.o: cminus.y globals.h 
	yacc -d cminus.y
	$(CC) $(CFLAGS) -c y.tab.c -lfl

lex.yy.o: cminus.l scan.h util.h globals.h
	flex cminus.l
	$(CC) $(CFLAGS) -c lex.yy.c -lfl

main.o: main.c globals.h util.h scan.h analyze.h cgen.h
	$(CC) $(CFLAGS) -c main.c

util.o: util.c util.h globals.h
	$(CC) $(CFLAGS) -c util.c

scan.o: scan.c scan.h util.h globals.h
	$(CC) $(CFLAGS) -c scan.c

#parse.o: parse.c parse.h scan.h globals.h util.h
#	$(CC) $(CFLAGS) -c parse.c

symtab.o: symtab.c symtab.h
	$(CC) $(CFLAGS) -c symtab.c

analyze.o: analyze.c globals.h symtab.h analyze.h
	$(CC) $(CFLAGS) -c analyze.c

code.o: code.c code.h globals.h
	$(CC) $(CFLAGS) -c code.c

cgen.o: cgen.c globals.h symtab.h code.h cgen.h
	$(CC) $(CFLAGS) -c cgen.c

clean:
	-rm cminus
	-rm $(OBJS)
	-rm y.tab.c
	-rm y.tab.h
	-rm lex.yy.c
#	-rm tm.o
#	-rm cminus_flex
#	-rm tm.exe

#tm.exe: tm.c
#	$(CC) $(CFLAGS) -o tm.c

#tiny: tiny.exe

#tm: tm.exe
