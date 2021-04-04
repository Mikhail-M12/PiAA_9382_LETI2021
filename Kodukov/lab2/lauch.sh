mkdir ./results/
for i in {1..8}
  do ./lab2 < ./tests/$i.txt > ./results/result$i.txt  
done