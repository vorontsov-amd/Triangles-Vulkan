cmake_minimum_required(VERSION 3.5)

project(Geometric)	

set(SOURCES triangle.cpp)
set(HEADERS triangle.hpp)

add_library(lib ${SOURCES} ${HEADERS})
target_include_directories(lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
