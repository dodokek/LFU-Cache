#!/bin/sh

# 1st cmd line argument - amount of tests
# 2nd cmd line argument - path to executable
# 3rd cmd line argument - folder name for answers


if [ "$1" ]; then
     echo "Performing $1 tests"
     tests_amount=$1
     i=0

     while [ "$i" -lt $tests_amount ]; do
	    echo "Running test $i" 
	    i=$(( i + 1 ))
	    $2 < ./tests/$i.txt > ./tests/$3/ans_$i.txt
     done
  
else
    echo "Please enter amount of tests to be runned"
    echo "Example of program usage:"
    echo "./run_tests.sh 10"
fi
