#!/bin/bash
echo "Running the program. If any error comes make sure you have created the build directory and run cmake .. command in it."
cd build/ && make && cd ..
echo -e "\n\n\n"
./build/app