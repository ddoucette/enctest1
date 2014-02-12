
MEM_SOURCES=unittest_mem.cpp    \
            ../common/logger.cpp    \
            ../common/memory.cpp    \
            ../encoder/FrameBuffer.cpp

EVENT_SOURCES=    unittest_event.cpp    \
                ../common/logger.cpp    \
                ../common/memory.cpp    \
                ../common/Benchmarking.cpp    \
                ../common/Events.cpp

DATA_SOURCES=    unittest_data.cpp    \
                ../common/logger.cpp    \
                ../common/memory.cpp    \
                ../common/Benchmarking.cpp    \
                ../common/DataSourceSink.cpp    \
                ../common/Events.cpp

DBUF_SOURCES=    unittest_dbuf.cpp    \
                ../common/logger.cpp    \
                ../common/memory.cpp    \
                ../common/Benchmarking.cpp    \
                ../common/dbuf.cpp


MEM_OBJECTS=$(MEM_SOURCES:.cpp=.o)
EVENT_OBJECTS=$(EVENT_SOURCES:.cpp=.o)
DATA_OBJECTS=$(DATA_SOURCES:.cpp=.o)
DBUF_OBJECTS=$(DBUF_SOURCES:.cpp=.o)

INCLUDES=-I../common -I../encoder -I/home/ddoucette/local/include
#CXX=clang++
CXX=g++
CFLAGS=-c -Wall -Werror -g -O2 -DDEBUG -std=c++0x
LDFLAGS=-lboost_date_time -L/home/ddoucette/local/lib
MEM=unittest_mem
EVENT=unittest_event
DATA=unittest_data
DBUF=unittest_dbuf

all: $(MEM_SOURCES) $(MEM) $(EVENT_SOURCES) $(EVENT) $(DBUF)
	
$(MEM): $(MEM_OBJECTS)
	$(CXX) $(MEM_OBJECTS) $(LDFLAGS) -o $@

$(EVENT): $(EVENT_OBJECTS)
	$(CXX) $(EVENT_OBJECTS) $(LDFLAGS) -o $@

$(DATA): $(DATA_OBJECTS)
	$(CXX) $(DATA_OBJECTS) $(LDFLAGS) -o $@

$(DBUF): $(DBUF_OBJECTS)
	$(CXX) $(DBUF_OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CXX) $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	rm -Rf *.o ../common/*.o ../encoder/*.o $(MEM) *~
