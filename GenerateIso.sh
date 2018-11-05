rm -rf build
mkdir build
cd build
cmake ..
make VERBOSE=${VERBOSE}
make install
cd ..
rm -rf iso

