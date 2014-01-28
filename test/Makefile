
SOURCES=main.cpp ../common/logger.cpp
OBJECTS=$(SOURCES:.cpp=.o)

INCLUDES=-I../common
CXX=clang++
CFLAGS=-c -Wall -Werror -g -O2
LDFLAGS=
EXECUTABLE=unittest

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CXX) $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	rm -Rf *.o $(EXECUTABLE) *~
