#! /usr/bin/bash

mkdir ./results/

for i in {1..4}
do
./lab5 < ./tests/s$i.txt > ./results/results$i.txt
done