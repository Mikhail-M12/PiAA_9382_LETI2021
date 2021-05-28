#! /usr/bin/bash
g++ ./src/lab5_1.cpp -o lab1
g++ ./src/lab5_2.cpp -o lab2

mkdir ./tests/

for i in {1..4}
do
echo "lab1 test $i"
./lab1 < ./tests/t$i
done
for i in {1..4}
do
echo "lab2 test $i"
./lab2 < ./tests/test$i
done