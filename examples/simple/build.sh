rm build/example
rm build/bin
mkdir build
cd build
cmake ..
make -j$(nproc) install
cd ..