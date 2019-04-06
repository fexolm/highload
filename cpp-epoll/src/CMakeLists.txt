cmake_minimum_required(VERSION 3.10)

FILE(GLOB SOURCES *.cpp *.h */*.cpp */*.h)

include_directories(${LIB_DIR} ${SRC_DIR})
add_executable(highload ${SOURCES})

target_link_libraries(highload -lboost_system -lboost_filesystem)