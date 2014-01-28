
SOURCES=main.cpp ../common/logger.cpp ../common/memory.cpp
OBJECTS=$(SOURCES:.cpp=.o)

INCLUDES=-I../common -I/Users/ddoucett/local/include
CXX=clang++
CFLAGS=-c -Wall -Werror -g -O2 -DDEBUG -std=c++11 -stdlib=libc++
LDFLAGS= -lboost_date_time -L.
EXECUTABLE=unittest

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CXX) $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	rm -Rf *.o ../common/*.o $(EXECUTABLE) *~
