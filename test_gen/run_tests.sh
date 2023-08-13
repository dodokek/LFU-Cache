#!/bin/sh


if [ "$1" ]; then
     echo "Performing $1 tests"
     tests_amount=$1
     i=0

     while [ "$i" -lt $tests_amount ]; do
	    echo "Running test $i" 
	    i=$(( i + 1 ))
	    ../LFU/main < ./tests/$i.txt
     done
  
else
    echo "Please enter amount of tests to be runned"
    echo "Example of program usage:"
    echo "./run_tests.sh 10"
fi
