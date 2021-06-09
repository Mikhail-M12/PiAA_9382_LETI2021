#! /usr/bin/bash

mkdir ./results/

for i in {1..4}
do
./lab5_2 < ./test2/$i.txt > ./results2/result$i.txt
done