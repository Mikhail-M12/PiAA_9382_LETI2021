
mkdir ./results/
for i in {1..3}
  do ./lab4  < ./tests/test$i.txt > ./results/test$i.txt  
done
