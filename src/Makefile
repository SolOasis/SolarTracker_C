AR       := ar
ARFLAGS  := cr
CXX      := g++
CPPFLAGS := -O2 -I../lib

all : directories executable

debug: CPPFLAGS += -DDEBUG -g
debug: directories executable

directories :
	@mkdir -p ../bin

executable : ../bin/spa_tester
	@echo -n ""

clean :
	rm *.o ../bin/spa_tester

../bin/spa_tester: spa_tester.o ../lib/spa.a
	$(CXX) $^ -o $@

spa_tester.o : spa_tester.c
	$(CXX) $(CPPFLAGS) -c $< -o $@
