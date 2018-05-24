y.tab.c: my-shell.y
	yacc -d my-shell.y

lex.yy.c: y.tab.h my-shell.l
	lex my-shell.l

my-shell: lex.yy.c y.tab.c
	g++ lex.yy.c y.tab.c -o my-shell

clean:
	rm -rf lex.yy.c y.tab.c y.tab.h calc
