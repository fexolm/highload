cmake_minimum_required(VERSION 3.10)
project(highload)

SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
SET (CMAKE_CXX_FLAGS                "-Wall -Werror -pthread")
SET (CMAKE_CXX_FLAGS_DEBUG          "-DPORT=8080 -O0 -g")
SET (CMAKE_CXX_FLAGS_RELEASE        "-DPORT=80 -Ofast -flto")

SET(CMAKE_CXX_STANDARD 17)


SET(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
SET(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
