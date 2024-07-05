cmake_minimum_required(VERSION 3.11)

project(
    tinystl
    VERSION 0.1
    DESCRIPTION "A tiny STL implementation"
    LANGUAGES CXX
)

add_subdirectory(src)

add_subdirectory(tests)