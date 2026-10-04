cmake_minimum_required(VERSION 4.4.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_MODULE_STD ON)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD f35a9ac6-8463-4d38-8eec-5d6008153e7d)

project(AandDSP2026 LANGUAGES CXX)

if(CMAKE_CXX_COMPILE_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILE_ID STREQUAL "GNU")
    add_compile_options(-Wall -Wconversion -Wextra -Wpedantic)
elseif(CMAKE_COMPILE_ID STREQUAL "MSVC")
    add_compile_options(/utf-8 /W4)
endif()