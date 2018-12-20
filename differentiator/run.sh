mkdir tmp
cd tmp
cmake ..
make
./differentiator ../$1
cat output.txt
