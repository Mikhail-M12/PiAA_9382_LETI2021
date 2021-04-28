p=20
for i in $(seq $p); do 
    ./lab1 < ./tests/$i.txt > ./results/result$i.txt
done
