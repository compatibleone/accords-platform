rm -rf build
mkdir build
cd build
../configure
cd backend
make -j4
cd ..
make
