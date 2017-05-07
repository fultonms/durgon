durgon: y.tab.o lex.yy.o tree.o node.o scope.o semmantic.o register.o gencode.o
	gcc -g -o durgon y.tab.o lex.yy.o tree.o node.o scope.o semmantic.o register.o gencode.o -ll -ly

gencode.o: gencode.c gencode.h
	gcc -g -c gencode.c

register.o: register.c register.h
	gcc -g -c register.c

semmantic.o: semmantic.c semmantic.h
	gcc -g -c semmantic.c

scope.o: scope.c scope.h
	gcc -g -c scope.c

node.o: node.c node.h
	gcc -g -c node.c

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
