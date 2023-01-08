all: example

CXX = g++-12.2 -std=gnu++20
CXXFLAGS = -O3
LFLAGS = -lquadmath -lm

example: example.cpp
	$(CXX) $(CXXFLAGS) example.cpp -o example.x $(LFLAGS)

clean:
	rm -rf example.x

