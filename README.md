# Cache

## Overview 

Here is my 2 different implementations of LFU cache and implementation of "Perfect" cache(Cache, awared of future queries).

## Build

To build the project, you need to execute the following commands:
```shell
cd LFU
mkdir build
cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
make --directory=build
```
You will get three executables:
- LFU_hashmap - O(1) realisation of LFU
- perfect_cache - realisation of perfect cache

## Testing

In this repository you can also find Python script for test generation and Shellscript for answers generation.

You can generate any amount(10 as example) of tests by:
```shell
mkdir tests
./test_get/gen_tests.py
> 10 
```
After that, you can run tests on executable by:
```shell
./test_gen/run_tests.sh <amount_of_tests> <path_to_exec> <path_to_ans_folder>
# example: ./run_tests.sh 10 ../LFU/build/perfect_cache ./perfect
```

To compare answers, you need to execute the following script

```bash
#/test_gen/

./compare.sh <tests_amount> <path_to_ans1_folder> <path_to_ans2_folder>
# example: ./compare.sh 9 ./tests/perfect ./tests/native
```

## Conclusion

I learned a lot of new C++ mechanics, thanks to this project. You can read more about O(1) realisation of LFU cache here: http://dhruvbird.com/lfu.pdf
