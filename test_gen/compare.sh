#!/bin/bash

# $1 - amount of tests
# $2 - path to native answers
# $3 - path to perfect answers

tests_amount=$1
i=0

while [ "$i" -lt $tests_amount ]; do
    echo "Running answers: $i" 
    i=$(( i + 1 ))
    diff --brief $2/ans_$i.txt $3/ans_$i.txt > /dev/null
    comp_value=$?

    if [ $comp_value -eq 1 ]
    then
        echo "Answers are different"
    else
        echo "Answers are the same"
    fi
    
done