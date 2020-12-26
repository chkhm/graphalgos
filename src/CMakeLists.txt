
cmake_minimum_required(VERSION 3.10)
include(GoogleTest)

# set the project name
project(graphalgos)
set(CMAKE_CXX_STANDARD 17)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

# add the executable
add_executable(graphalgos arg_parser.cpp edge.cpp element.cpp graph.cpp main.cpp node.cpp)
add_executable(graphtest test_graph.cpp edge.cpp element.cpp graph.cpp node.cpp)
target_link_libraries(graphtest gtest gtest_main)
gtest_add_tests(TARGET      graphtest
                TEST_SUFFIX .noArgs
                TEST_LIST   noArgsTests
)

