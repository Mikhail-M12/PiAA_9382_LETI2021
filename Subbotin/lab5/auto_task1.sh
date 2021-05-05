#! /usr/bin/bash

mkdir ./results/

for i in {1..4}
do
./lab5 < ./tests/f$i.txt > ./results/resultf$i.txt
done