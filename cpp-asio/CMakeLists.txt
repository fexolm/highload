cmake_minimum_required(VERSION 3.10)
project(highload)

SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
SET (CMAKE_CXX_FLAGS                "-Wall -Werror -pthread")
SET (CMAKE_CXX_FLAGS_DEBUG          "-DPORT=8080 -O0 -g")
SET (CMAKE_CXX_FLAGS_RELEASE        "-DPORT=80 -Ofast -flto")

SET(CMAKE_CXX_STANDARD 17)

FILE(GLOB SOURCES "*.cpp")

FIND_PACKAGE(Boost 1.42.0 REQUIRED COMPONENTS system thread regex)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

add_executable(highload ${SOURCES})

SET(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})

TARGET_LINK_LIBRARIES(highload ${USED_LIBS})

