project(testTriangle)

file(GLOB CPP_SOURCES ../vulkan/GeomLib/src/*.cpp)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${CPP_SOURCES} ./src/gtest.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ../vulkan/GeomLib/include/)
target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})

add_definitions(-DPROJECT_DIR_PATH=\"${CMAKE_SOURCE_DIR}\")

execute_process(
        COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/10000.1.txt ${CMAKE_INSTALL_BINDIR}
)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT ${PROJECT_NAME})