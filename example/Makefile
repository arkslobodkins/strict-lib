all: example1 example2

CXX = g++-12.1
CXXFLAGS = -std=gnu++20 -O3
LFLAGS = -lquadmath -lm

example1: example1.cpp
	$(CXX) $(CXXFLAGS) example1.cpp -o example1.x $(LFLAGS)

example2: example2.cpp
	$(CXX) $(CXXFLAGS) example2.cpp -o example2.x $(LFLAGS) -DSTRICT_DEBUG_OFF

clean:
	rm -rf example1.x example2.x
