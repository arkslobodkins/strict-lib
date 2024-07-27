#! /bin/bash
make all -j8

echo -e "\nRUNNING EXAMPLE 0"
./example0.x

echo -e "\nRUNNING EXAMPLE 1"
./example1.x

echo -e "\nRUNNING EXAMPLE 2"
./example2.x

echo -e "\nRUNNING EXAMPLE 3"
./example3.x

echo -e "\nCLEAN EXAMPLES"
make clean
echo -e ""
