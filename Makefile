all: example

CXX = g++-12.1
CXXFLAGS = -std=gnu++20 -O3 -DSTRICT_DEBUG_ON
LFLAGS = -lquadmath -lm

example: example.cpp
	$(CXX) $(CXXFLAGS) example.cpp -o example.x $(LFLAGS)

clean:
	rm -rf example.x

