cmake_minimum_required(VERSION 3.10)

FILE(GLOB SOURCES *.cpp *.h)

add_executable(highload ${SOURCES} Request.cpp Request.h)