#! /usr/bin/bash

mkdir ./results/

for i in {1..5}
do
./main < ./test/$i.txt > ./results/result$i.txt
done