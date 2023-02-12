project(triangle)

aux_source_directory(./src CPP_SOURCES)

add_executable(${PROJECT_NAME} ${CPP_SOURCES} main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ./include)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT ${PROJECT_NAME})
