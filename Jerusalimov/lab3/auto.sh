#! /usr/bin/bash

mkdir ./results/

for i in {1..15}
do
./main < ./test/$i.txt > ./results/result$i.txt
done