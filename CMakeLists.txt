cmake_minimum_required(VERSION 3.10)
project(math_parser VERSION 1.0.0 LANGUAGES CXX)

add_library(math_parser_lib STATIC
    src/mathparser.cpp
)

target_include_directories(math_parser_lib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_executable(math_parser src/main.cpp)
target_link_libraries(math_parser PRIVATE math_parser_lib)

include(CTest)
add_executable(run_tests tests/test_mathparser.cpp)
target_link_libraries(run_tests PRIVATE math_parser_lib)
add_test(NAME math_parser_tests COMMAND run_tests)