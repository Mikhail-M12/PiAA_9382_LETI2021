mkdir ./res
for i in {1..6}
	do ./lab2 < ./tests/test$i.txt > ./res/res$i.txt
done
