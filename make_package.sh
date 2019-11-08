# !/bin/bash

mkdir flood;
cd flood; mkdir etc bin;
cp ../build/flood bin; cp ../etc/* etc
cd bin; mkdir cap
cd ../../; tar -zcvf flood.tgz flood
