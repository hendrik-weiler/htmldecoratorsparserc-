cmake_minimum_required(VERSION 3.19)
project(htmldecoratorparserc__)

set(CMAKE_CXX_STANDARD 11)

file(COPY testdata DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(htmldecoratorparserc__
        src/Parser.cpp
        main.cpp)