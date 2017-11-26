CC=g++ -g
CFLAGS=-c -Wall -Wextra -ansi -std=c++11 -Wno-unused-parameter -Wno-unused-function 
LDFLAGS=
SOURCES=src/FA_tools.cpp src/ToGraph.cpp src/ExpressionRationnelle.cpp src/Accept.cpp src/Determinize.cpp src/Equivalence.cpp src/Minimize.cpp src/parser.tab.cpp src/lex.yy.cpp src/FA_main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=ndet

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

parser: src/parser.tab.cpp src/lex.yy.cpp

src/parser.tab.cpp: 
	bison -d src/parser.y --output-file=src/parser.tab.cpp

src/lex.yy.cpp: src/parser.lex
	flex -o src/lex.yy.cpp src/parser.lex 

clean:
	rm -f ndet src/*.o src/*~ *~ 

cleanparser:
	# rm -f src/lex.yy.cpp src/location.hh src/parser.tab.cpp src/parser.tab.hpp src/position.hh src/stack.hh
	rm -f src/lex.yy.cpp src/parser.tab.cpp
