
LIBSOURCES = SurfaceEncoder.cpp unittest.cpp

OBJS=$(LIBSOURCES:.cpp=.o)

DEFS = -DHAVE_CONFIG_H
#CFLAGS = -g -O2 -Wall -Werror -std=c++11
CFLAGS = -g -O2 -Wall -Werror -std=c++0x
DEFAULT_INCLUDES = -I.
CC = clang
#CXX = clang++
CXX = g++

CCOMPILE = $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(CFLAGS)
CXXCOMPILE = $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(CFLAGS)

unittest: $(OBJS)
	$(CXXCOMPILE) -o unittest $(OBJS)

%.o: %.c
	$(CCOMPILE) -c -o $@ $<

%.o: %.cpp
	$(CXXCOMPILE) -c -o $@ $<
