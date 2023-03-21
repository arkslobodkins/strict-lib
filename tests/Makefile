all: test_types test_val test_array test_slicearray test_randslicearray test_functions

CXX = g++-12.1 -std=gnu++20
CXXFLAGS = -fsanitize=address
LFLAGS = -lquadmath -lm

test_types: test_types.cpp
	$(CXX) $(CXXFLAGS) test_types.cpp -o test_types.x $(LFLAGS)

test_val: test_val.cpp
	$(CXX) $(CXXFLAGS) test_val.cpp -o test_val.x $(LFLAGS)

test_array: test_array.cpp
	$(CXX) $(CXXFLAGS) test_array.cpp -o test_array.x $(LFLAGS)

test_slicearray: test_slicearray.cpp
	$(CXX) $(CXXFLAGS) test_slicearray.cpp -o test_slicearray.x $(LFLAGS)

test_randslicearray: test_randslicearray.cpp
	$(CXX) $(CXXFLAGS) test_randslicearray.cpp -o test_randslicearray.x $(LFLAGS)

test_functions: test_functions.cpp
	$(CXX) $(CXXFLAGS) test_functions.cpp -o test_functions.x $(LFLAGS)

clean:
	rm -rf test_types.x test_val.x test_array.x test_slicearray.x test_randslicearray.x test_functions.x

