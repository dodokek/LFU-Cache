# Cache

## Overview 

Here is my 2 different implementations of LFU cache and implementation of "Perfect" cache(Cache, awared of future queries).

## Build

To build all three caches you need to execute the following commands:
```shell
cd LFU
mkdir build
cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
make --directory=build
```
You will get three executables:
- LFU_cringe - simple realisation of LFU
- LFU_hashmap - O(1) realisation of LFU
- perfect_cache - realisation of perfect cache

## Testing

*still in progress*

In this repository you can also find Python script for test generation and Shellscript for answers generation.

You can generate any amount(10 as example) of tests by:
```shell
./test_get/gen_tests.py
> 10 
```
After that, you can run tests on executable by:
```shell
./test_gen/run_tests.sh <amount_of_tests> <path_to_exec>
```

