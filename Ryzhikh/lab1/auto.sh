#! /usr/bin/bash

mkdir ./results/

for i in {2..20}
do
./main < ./test/$i.txt > ./results/result$i.txt
done