a.out:  
	bison --yacc --defines --output=grammar.c grammar.y
	flex --outfile=al.c al.l
	gcc  grammar.c al.c SymbolTable.c utilities.c finalCode.c

clean:
	rm grammar.c al.c
	rm grammar.h
	rm *.out
