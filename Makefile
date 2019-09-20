CC = clang++
DEBUG = -g
CPPFLAGS=$(shell llvm-config --cxxflags)
LDFLAGS=$(shell llvm-config --ldflags --libs)

swi2else: lex.yy.o parser.o ast.o
	$(CC) $(LDFLAGS) -o $@ $^
lex.yy.o: lex.yy.c parser.tab.hpp
	$(CC) $(CPPFLAGS) -Wno-deprecated $(DEBUG) -c -o $@ $<
lex.yy.c: lexer.lex
	flex $<
parser.o: parser.tab.cpp parser.tab.hpp
	$(CC) $(CPPFLAGS) -c  $(DEBUG) -o $@ $<
parser.tab.cpp parser.tab.hpp: parser.ypp
	bison -v -d $<
ast.o: ast.cpp ast.hpp
	$(CC) $(CPPFLAGS) -c $(DEBUG) -o $@ $<

.PHONY: clean

clean:
	rm -f *~ *tab* lex.yy.c parser.output swi2else *.o *.out tests/*.ll tests/*.s

