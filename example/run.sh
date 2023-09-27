#! /bin/bash
make all -j4
echo -e "\nRUNNING EXAMPLE1"
./example1.x

echo -e "\nRUNNING EXAMPLE2"
./example2.x

echo -e "\nRUNNING EXAMPLE3"
./example3.x

echo -e "\nRUNNING EXAMPLE4"
./example4.x

echo -e "\nRUNNING EXAMPLE5"
./example5.x

echo -e "\nRUNNING EXAMPLE6"
./example6.x

echo -e "\nCLEAN EXAMPLES"
make clean
echo -e ""
