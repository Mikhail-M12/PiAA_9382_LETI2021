#! /usr/bin/bash

mkdir ./results/

for i in {2..30}
do
./lab1 < ./tests/$i.txt > ./results/result$i.txt
done