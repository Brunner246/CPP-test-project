cmake_minimum_required(VERSION 3.27)
project(Test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Test main.cpp)
target_compile_features(Test PRIVATE cxx_std_20)

add_executable(effective-modern-cpp effective-main.cpp
        test.iterator.ixx)
target_compile_features(effective-modern-cpp PRIVATE cxx_std_20)