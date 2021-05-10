mkdir ./results/
mkdir ./results/1
mkdir ./results/2

for i in {1..5}
do 
./lab4_1 < ./tests/step1/$i.txt > ./results/1/result$i.txt  
done

for i in {1..5}
do 
./lab4_2 < ./tests/step2/$i.txt > ./results/2/result$i.txt  
done