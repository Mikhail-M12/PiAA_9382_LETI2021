#! /usr/bin/bash

mkdir -p ./results/

for i in {1..6}
do
./asterisk < ./test/$i.txt > ./results/result$i.txt
done
