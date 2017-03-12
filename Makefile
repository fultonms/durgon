durgon: y.tab.o lex.yy.o tree.o
	gcc -g -o durgon y.tab.o lex.yy.o tree.o -ll -ly

tree.o: tree.c tree.h
	gcc -g -c tree.c

y.tab.o: y.tab.c
	gcc -g -c y.tab.c

lex.yy.o: lex.yy.c
	gcc -g -c lex.yy.c

y.tab.c: durgon.y
	yacc -dv durgon.y

lex.yy.c: durgon.l
	lex -l durgon.l

clean:
	rm -f *.o durgon y.tab.c lex.yy.c 
