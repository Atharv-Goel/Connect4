mkdir build
cd build
cmake -DCMAKE_CXX_COMPILER=$(which clang++) ..
cmake --build .
./Connect4