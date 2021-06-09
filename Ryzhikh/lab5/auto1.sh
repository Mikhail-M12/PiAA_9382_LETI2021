#! /usr/bin/bash

mkdir ./results/

for i in {1..3}
do
./lab5_1 < ./test1/$i.txt > ./results1/result$i.txt
done