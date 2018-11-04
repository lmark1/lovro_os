rm -rf build
rm -rf iso
mkdir build
cd build
cmake ..
make VERBOSE=${VERBOSE}
make install
cd ..
rm -rf iso

