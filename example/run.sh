#! /bin/bash
make all -j4
echo -e "\nRUNNING EXAMPLE1"
./example1.x

echo -e "\nRUNNING EXAMPLE2"
./example2.x

echo -e "\nCLEAN EXAMPLES"
make clean
echo -e ""
