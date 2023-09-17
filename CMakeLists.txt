cmake_minimum_required(VERSION 3.26)
project(passmx)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(include)
include_directories(include/passmx)

add_executable(passmx
        include/passmx/passmx.h
        include/passmx/token.h
        source/passmx/passmx.cpp
        source/passmx/main.cpp)
