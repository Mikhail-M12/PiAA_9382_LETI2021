mkdir output;
for i in 1 2 3 4 5 6 
do ./lab4 < tests/$i.txt > output/out$i.txt
done;