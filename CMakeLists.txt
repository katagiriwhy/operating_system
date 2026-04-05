cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(operating_system LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(rtos_core STATIC
        src/task/task.cpp
        src/schedular/schedular.cpp
        src/semaphore/semaphore.cpp
)

target_include_directories(rtos_core PUBLIC
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/task
        ${CMAKE_SOURCE_DIR}/src/schedular
)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE rtos_core)

add_executable(rtos_tests tests/rtos_tests.cpp)
target_link_libraries(rtos_tests PRIVATE rtos_core)

enable_testing()
add_test(NAME rtos_tests COMMAND rtos_tests)
