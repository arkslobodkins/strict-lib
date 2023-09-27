#! /bin/bash
make all -j4
./test_types.x
./test_val.x
./test_slicearray.x
./test_randslicearray.x
./test_array.x
./test_functions.x
./test_expr.x
make clean
