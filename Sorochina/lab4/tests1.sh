mkdir ./res
for i in {1..6}
	do ./lab4_1 < ./tests/test$i.txt > ./res/res1_$i.txt
done