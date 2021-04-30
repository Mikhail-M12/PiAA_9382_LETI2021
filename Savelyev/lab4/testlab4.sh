
mkdir ./results/
for i in {1..2}
  do ./lab41  < ./tests/test$i.txt > ./results/test$i.txt
done
    ./lab42  < ./tests/test3.txt > ./results/test3.txt

