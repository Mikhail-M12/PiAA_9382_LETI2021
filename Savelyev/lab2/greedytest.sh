mkdir ./results/
for i in {1..3}
  do ./labgreedy2  < ./tests/greedy$i.txt > ./results/gresult$i.txt  
done
