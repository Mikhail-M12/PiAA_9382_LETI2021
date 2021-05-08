#!/bin/bash
function run() {
	i=1
	t=$1
	prog=$2
	shift
	shift
	for var in $@
	do
		echo Test $i $t
		i=$((i+1))
		cat $var
		$prog < $var
		echo 
	done
}
tests1=(tests/test1.txt tests/test3.txt  tests/test5.txt)
tests2=(tests/test2.txt tests/test4.txt)
run "KMP" ./kmp ${tests1[@]} 
run "Circular" ./circular ${tests2[@]}
