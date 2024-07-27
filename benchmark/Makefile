compiler = gcc

all: array1D fixedarray1D

LFLAGS = -lm -lbenchmark -lpthread

CXXFLAGS=-std=c++20 -O3 -no-pie

ifeq ($(compiler), gcc)
CXX = g++-12.3
CXXFLAGS += -march=native
exe1 = array_gcc.x
exe2 = fixedarray_gcc.x

else ifeq ($(compiler), icpx)
CXX = icpx
CXXFLAGS += -xHost -fp-model=precise #-xcore-avx512 -fno-alias
exe1 = array_icpx.x
exe2 = fixedarray_icpx.x

else ifeq ($(compiler), clang)
CXX = clang++
CXXFLAGS += -march=native
exe1 = array_clang.x
exe2 = fixedarray_clang.x
endif

array1D: array1D_bm.cpp
	$(CXX) $(CXXFLAGS) array1D_bm.cpp -o $(exe1) -DSTRICT_DEBUG_OFF -DEIGEN_NO_DEBUG $(LFLAGS)

fixedarray1D: fixedarray1D_bm.cpp
	$(CXX) $(CXXFLAGS) fixedarray1D_bm.cpp -o $(exe2) -DSTRICT_DEBUG_OFF -DEIGEN_NO_DEBUG $(LFLAGS)

clean:
	rm -rf *.x
