#!/bin/sh

# $1 - name of project ot be tested and compared with perfect cache
tests_amount=9

# Generating tests
python3 ./gen_tests.py

# Performing tests on $1 

./run_tests.sh $tests_amount $1

# Performing tests on perfect cache

./run_tests.sh $tests_amount perfect_cache

# Compating results

./compare.sh $tests_amount ./tests/perfect_cache ./tests/$1 
