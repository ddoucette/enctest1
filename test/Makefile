
SOURCES=main.cpp ../common/logger.cpp ../common/memory.cpp ../encoder/FrameBuffer.cpp ../common/DataSourceSink.cpp

OBJECTS=$(SOURCES:.cpp=.o)

INCLUDES=-I../common -I../encoder -I/home/ddoucette/local/include
#CXX=clang++
CXX=g++
CFLAGS=-c -Wall -Werror -g -O2 -DDEBUG -std=c++0x
LDFLAGS=-lboost_date_time -L/home/ddoucette/local/lib
EXECUTABLE=unittest

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CXX) $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	rm -Rf *.o ../common/*.o ../encoder/*.o $(EXECUTABLE) *~
