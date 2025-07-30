cmake_minimum_required(VERSION 3.10)
project(math_parser)

include(CTest)

# main programm
add_executable(math_parser 
    src/main.cpp 
    src/mathparser.cpp
)

# tests
add_executable(run_tests tests/test_mathparser.cpp src/mathparser.cpp)
add_test(NAME run_tests
         COMMAND run_tests)
