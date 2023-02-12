project(testTriangle)

file(GLOB CPP_SOURCES ../triangles/src/*.cpp)

add_executable(${PROJECT_NAME} ${CPP_SOURCES} ./src/gtest.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ../triangles/include)
target_link_libraries(${PROJECT_NAME} gtest gmock pthread)

add_definitions(-DPROJECT_DIR_PATH=\"${CMAKE_SOURCE_DIR}\")

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT ${PROJECT_NAME})