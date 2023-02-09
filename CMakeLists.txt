cmake_minimum_required(VERSION 3.5)

project(Triangles)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O2")

include_directories("${PROJECT_SOURCE_DIR}/src/lib")

add_library(${PROJECT_NAME}_lib STATIC src/lib/triangle.cpp src/lib/intersect.cpp src/lib/octree.cpp src/lib/vector.cpp src/lib/plane.cpp)


target_include_directories(${PROJECT_NAME}_lib PUBLIC ${PROJECT_SOURCE_DIR}/src/lib)

file(GLOB CPP_SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${CPP_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)
