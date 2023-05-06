#! /bin/bash
make all -j4
echo -e "\nRUNNING EXAMPLE1"
./example1.x
echo -e "\nCLEAN EXAMPLE1"
make clean
echo -e ""
