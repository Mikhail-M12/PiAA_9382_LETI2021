#! /usr/bin/bash

mkdir ./results/

for i in {1..6}
do
./lab3 < ./tests/$i.txt > ./results/result$i.txt
done