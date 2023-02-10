cmake_minimum_required(VERSION 3.5)

project(Geometric)	

set(SOURCES triangle.cpp)
set(HEADERS triangle.hpp)
set(SOURCES vector.cpp)
set(HEADERS vector.hpp)
set(SOURCES plane.cpp)
set(HEADERS plane.hpp)
set(HEADERS line.hpp)
set(SOURCE line.cpp)
set(SOURCE segment.cpp)
set(HEADERS utility.hpp)
set(HEADERS segment.hpp)
set(HEADERS octree.hpp)
set(SOURCES octree.cpp)
set(SOURCES intersect.cpp)
set(HEADERS intersect.hpp)



add_library(lib ${SOURCES} ${HEADERS})
target_include_directories(lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
