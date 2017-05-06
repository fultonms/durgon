durgon: y.tab.o lex.yy.o tree.o gencode.o
	gcc -g -o durgon y.tab.o lex.yy.o tree.o gencode.o -ll -ly

gencode.o: gencode.c gencode.h
	gcc -g -c gencode.c

tree.o: tree.c tree.h
	gcc -g -c tree.c

y.tab.o: y.tab.c
	gcc -g -c y.tab.c

lex.yy.o: lex.yy.c
	gcc -g -c lex.yy.c

y.tab.c: parser.y
	yacc -dv parser.y

lex.yy.c: scanner.l
	lex -l scanner.l

clean:
	rm -f *.o durgon y.tab.c lex.yy.c y.tab.h y.output
