from os import path
from random import randint

CACHE_CAPACITY_MIN = 10
CACHE_CAPACITY_MAX = 200
INPUT_SIZE_MIN     = 100
INPUT_SIZE_MAX     = 10000


def generate_test(filename, tests_amount):
    with open(path.join("tests", filename), "w") as test_file:
        
        cache_capacity = randint(CACHE_CAPACITY_MIN, CACHE_CAPACITY_MAX)
        input_size     = randint(INPUT_SIZE_MIN, INPUT_SIZE_MAX)
        test_file.write(f"{cache_capacity} {input_size}\n")

        for i in range(input_size):
            single_value = randint(-100000, 100000) 
            test_file.write(f"{single_value} ")
    

def start_generation():
    tests_amount = 10

    for test in range(tests_amount):
        print ("Generating test:", test + 1)

        filename = f"{test}.txt"
        generate_test(filename, tests_amount)


if __name__ == "__main__":
    start_generation()
