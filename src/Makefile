AR       := ar
ARFLAGS  := cr
CXX      := g++
CPPFLAGS := -O2 -I../lib

all : directories executable

debug: CPPFLAGS += -DDEBUG -g
debug: directories executable test

directories :
	@mkdir -p ../bin

executable : ../bin/SunTracker
	@echo -n ""

test : ../bin/spa_tester
	@echo -n ""

clean :
	rm *.o ../bin/spa_tester
	rm *.o ../bin/SunTracker

../bin/SunTracker: SunTracker.o ../lib/spa.a
	$(CXX) $^ -o $@
	
SunTracker.o : SunTracker.c
	$(CXX) $(CPPFLAGS) -c $< -o $@

../bin/spa_tester: spa_tester.o ../lib/spa.a
	$(CXX) $^ -o $@

spa_tester.o : spa_tester.c
	$(CXX) $(CPPFLAGS) -c $< -o $@
