# Makefile - CS6015 HW1
#
# To build a target, enter:
# make <target>
#
# Target:
# all - build everything
# clean - delete derived files from make


CXXSOURCE = main.cpp cmdline.cpp expr.cpp val.cpp parse.cpp tests.cpp
CXXSOURCE_test = testDriver.cpp exec.cpp

HEADERS = cmdline.hpp expr.hpp val.hpp catch.h parse.hpp
HEADERS_test = testDriver.hpp exec.h

CXXFLAGS = -std=c++11 -fsanitize=undefined -fno-sanitize-recover=undefined

all: msdscript

msdscript: $(CXXSOURCE) $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $(CXXSOURCE)
	$(CXX) $(CXXFLAGS) -o msdscript $(CXXSOURCE)

test_msdscript: $(CXXSOURCE_test) $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $(CXXSOURCE_test)
	$(CXX) $(CXXFLAGS) -o test_msdscript $(CXXSOURCE_test)


.PHONY: clean

clean:
	rm -f *.o msdscript test_msdscript

.PHONY: test

test: msdscript
	./msdscript --test

.PHONY: doc

doc:
	cd documentation && doxygen

