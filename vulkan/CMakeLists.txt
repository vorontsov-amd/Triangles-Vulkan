set(PROJECT_NAME SimpleEngine)
project(${PROJECT_NAME})

add_subdirectory(SimpleEngineCore)
add_subdirectory(SimpleEngineEditor)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SimpleEngineEditor)

