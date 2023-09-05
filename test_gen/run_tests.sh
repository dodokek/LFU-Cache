#!/bin/sh

# 1st cmd line argument - amount of tests
# 2nd cmd line argument - name of project


if [ "$1" ]; then
    # building project
    make --directory=../Cache/build

    # performing tests
    echo "Performing $1 tests"
    tests_amount=$1
    i=0

    mkdir -p tests/$2

    while [ "$i" -lt $tests_amount ]; do
        echo "Running test $i" 
        i=$(( i + 1 ))
        ../LFU/build/$2 < ./tests/$i.txt > ./tests/$2/ans_$i.txt
    done
  
else
    echo "Please enter amount of tests to be runned"
    echo "Example of program usage:"
    echo "./run_tests.sh 10 LFU_hashmap"
fi
