mycalc: y.tab.o lex.yy.o tree.o
	gcc -g -o mycalc y.tab.o lex.yy.o tree.o -ll -ly

tree.o: tree.c tree.h
	gcc -g -c tree.c

y.tab.o: y.tab.c
	gcc -g -c y.tab.c

lex.yy.o: lex.yy.c
	gcc -g -c lex.yy.c

y.tab.c: calc.y
	yacc -dv calc.y

lex.yy.c: calc.l
	lex -l calc.l

clean:
	rm -f *.o mycalc y.tab.c lex.yy.c

