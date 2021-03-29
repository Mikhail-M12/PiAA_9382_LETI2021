#! /usr/bin/bash

mkdir ./results/

for i in {2..6}
do
python3 ./src/l3.py < ./tests/$i.txt > ./results/result$i.txt
done
