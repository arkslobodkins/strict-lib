#! /bin/bash
make all -j4
echo -e "\nRUNNING EXAMPLE1\n"
./example1.x
echo -e "\nRUNNING EXAMPLE2\n"
./example2.x
make clean
