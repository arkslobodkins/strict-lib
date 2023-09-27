all: example1 example2 example3 example4 example5 example6

CXX = g++-13.1
CXXFLAGS = -std=gnu++20
LFLAGS = -lquadmath -lm

example1: example1.cpp
	$(CXX) $(CXXFLAGS) example1.cpp -o example1.x $(LFLAGS)

example2: example2.cpp
	$(CXX) $(CXXFLAGS) example2.cpp -o example2.x $(LFLAGS)

example3: example3.cpp
	$(CXX) $(CXXFLAGS) example3.cpp -o example3.x $(LFLAGS)

example4: example4.cpp
	$(CXX) $(CXXFLAGS) -O3 example4.cpp -o example4.x -DSTRICT_DEBUG_OFF $(LFLAGS)

example5: example5.cpp
	$(CXX) $(CXXFLAGS) -O3 example5.cpp -o example5.x $(LFLAGS)

example6: example6.cpp
	$(CXX) $(CXXFLAGS) -O3 example6.cpp -o example6.x -DSTRICT_DEBUG_OFF $(LFLAGS)

clean:
	rm -rf example1.x example2.x example3.x example4.x example5.x example6.x ex2_results.txt
