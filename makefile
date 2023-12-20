OS := $(shell uname)
ifeq ($(OS),Darwin)
	CC	= /opt/homebrew/bin/g++-13
	CFLAGS  = -O3 -std=c++14 -w -march=native -I/opt/homebrew/opt/boost/include
   	LDFLAGS =
else
	CC      = g++
	CFLAGS  = -O3 -mavx -std=c++14 -w
	LDFLAGS =
endif

SOURCES = utils.cpp containers/relation.cpp containers/offsets_templates.cpp containers/offsets.cpp containers/buffer.cpp indices/hierarchicalindex.cpp indices/hint_m_dynamic_naive.cpp indices/hint_m_dynamic_sec_attr.cpp indices/hint_m_dynamic.cpp indices/live_index.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: query

query: pureLIT teHINT  aLIT 

pureLIT: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) utils.o containers/relation.o containers/buffer.o indices/hierarchicalindex.o indices/live_index.o indices/hint_m_dynamic.o main_pureLIT.cpp -o query_pureLIT.exec $(LDADD)

teHINT: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) utils.o containers/relation.o indices/hierarchicalindex.o indices/hint_m_dynamic_naive.o main_teHINT.cpp -o query_teHINT.exec $(LDADD)

aLIT: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) utils.o containers/relation.o containers/buffer.o indices/hierarchicalindex.o indices/live_index.o indices/hint_m_dynamic_sec_attr.o main_aLIT.cpp -o query_aLIT.exec $(LDADD)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf utils.o
	rm -rf containers/*.o
	rm -rf indices/*.o
	rm -rf query_teHINT.exec
	rm -rf query_pureLIT.exec
	rm -rf query_aLIT.exec

