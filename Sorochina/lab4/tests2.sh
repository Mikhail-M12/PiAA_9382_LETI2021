mkdir ./res
for i in {1..6}
    do ./lab4_2 < ./tests/test$i.txt > ./res/res2_$i.txt
done