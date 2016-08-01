# SiW-Calorimeter-Simulation
# build and compile
mkdir build bin lib

source /path/to/ilcsoft/init_ilcsoft.sh

cd build
cmake -C ${ILCSOFT}/ILCSoft.cmake ../
make install 
cd ..	

