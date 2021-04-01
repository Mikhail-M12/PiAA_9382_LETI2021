mkdir ./results/

cd src/
make
cd ../
for i in {1..2}
do
./src/lab3 < ./test/test$i.txt > ./results/result$i.txt
done

cd src/
make clean