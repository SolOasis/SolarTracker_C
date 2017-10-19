AR       := ar
ARFLAGS  := cr
CXX      := g++
CPPFLAGS := -O2

all : spa.a
	@echo -n ""

clean :
	rm *.a *.o

spa.a : spa.o
	$(AR) $(ARFLAGS) $@ $^ 

spa.o : spa.c spa.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

