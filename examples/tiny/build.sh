rm build/example
mkdir build
cd build
cmake ..
make -j$(nproc) install
cd ..