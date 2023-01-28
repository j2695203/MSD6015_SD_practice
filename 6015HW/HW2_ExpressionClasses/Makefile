# Makefile - CS6015 HW1
#
# To build a target, enter:
# make <target>
#
# Target:
# all - build everything
# clean - delete derived files from make


CXXSOURCE = main.cpp cmdline.cpp expr.cpp

HEADERS = cmdline.hpp expr.hpp catch.h

CXXFLAGS = -std=c++11

all: msdscript

msdscript: $(CXXSOURCE) $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $(CXXSOURCE)
	$(CXX) $(CXXFLAGS) -o msdscript $(CXXSOURCE)


.PHONY: clean

clean:
	rm -f *.o msdscript

