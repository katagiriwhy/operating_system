cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(operating_system)

include( ${PROJECT_SOURCE_DIR}/src/src.cmake )

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
        PRIVATE
        ${PROJECT_SOURCE_DIR}/src/main.cpp
        ${${PROJECT_NAME}-sources}
)

target_include_directories( ${PROJECT_NAME}
        PRIVATE
        ${${PROJECT_NAME}-includes}
)

