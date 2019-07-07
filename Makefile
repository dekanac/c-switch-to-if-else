CC = clang++
CPPFLAGS=$(shell llvm-config --cxxflags)
LDFLAGS=$(shell llvm-config --ldflags --libs)

swi2else: lex.yy.o parser.o ast.o
	$(CC) $(LDFLAGS) -o $@ $^
lex.yy.o: lex.yy.c parser.tab.hpp
	$(CC) $(CPPFLAGS) -Wno-deprecated -c -o $@ $<
lex.yy.c: lexer.lex
	flex $<
parser.o: parser.tab.cpp parser.tab.hpp
	$(CC) $(CPPFLAGS) -c -o $@ $<
parser.tab.cpp parser.tab.hpp: parser.ypp
	bison -v -d $<
ast.o: ast.cpp ast.hpp
	$(CC) $(CPPFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -f *~ *tab* lex.yy.c swi2else *.o

