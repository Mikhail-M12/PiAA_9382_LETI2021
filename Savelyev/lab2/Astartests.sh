mkdir ./results/
for i in {1..3}
  do ./labastar2  < ./tests/Astar$i.txt > ./results/aresult$i.txt  
done
