all: example2

CXX = g++-12.2
CXXFLAGS = -std=gnu++20
LFLAGS = -lquadmath -lm

example2: example2.cpp
	$(CXX) $(CXXFLAGS) example2.cpp -o example2.x $(LFLAGS)

clean:
	rm -rf example.x

