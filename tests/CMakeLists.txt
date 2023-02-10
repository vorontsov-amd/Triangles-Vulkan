project(testTriangle)

aux_source_directory(./src CPP_SOURCES)

add_executable(${PROJECT_NAME} ${CPP_SOURCES})

target_link_libraries(${PROJECT_NAME} gtest gmock pthread)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT ${PROJECT_NAME})