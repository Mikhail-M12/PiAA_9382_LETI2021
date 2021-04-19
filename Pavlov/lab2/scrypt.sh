mkdir output;
for i in 1 2 3 4 
do ./lab2 < tests/$i.txt > output/out$i.txt
done;