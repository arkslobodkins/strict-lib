debug = 0

all: example0 example1 example2 example3

CXX = g++-13.2

CXXFLAGS1 = -std=c++20 -Wconversion -Wnarrowing
LFLAGS1 = -lm

CXXFLAGS2 = -std=gnu++20 -O3 -Wconversion -Wnarrowing -DSTRICT_QUAD_PRECISION
LFLAGS2 = -lm -lquadmath

IPATH=-I ../src/

ifeq ($(debug), 0)
CXXFLAGS1 += -DSTRICT_DEBUG_OFF
CXXFLAGS2 += -DSTRICT_DEBUG_OFF
endif

ifeq ($(debug), 1)
CXXFLAGS1 += -g -fsanitize=address
CXXFLAGS2 += -g -fsanitize=address
endif

example0: example0.cpp
	$(CXX) $(CXXFLAGS1) example0.cpp -o example0.x $(LFLAGS1) $(IPATH)

example1: example1.cpp
	$(CXX) $(CXXFLAGS1) example1.cpp -o example1.x $(LFLAGS1) $(IPATH)

example2: example2.cpp
	$(CXX) $(CXXFLAGS1) example2.cpp -o example2.x $(LFLAGS1) $(IPATH)

example3: example3.cpp
	$(CXX) $(CXXFLAGS2) example3.cpp -o example3.x $(LFLAGS2) $(IPATH)

clean:
	rm -rf *.x *.txt
