list( APPEND ${PROJECT_NAME}-includes
        ${CMAKE_CURRENT_LIST_DIR}/
        ${CMAKE_CURRENT_LIST_DIR}/task/
        ${CMAKE_CURRENT_LIST_DIR}/schedular/
)

list( APPEND ${PROJECT_NAME}-sources
        ${CMAKE_CURRENT_LIST_DIR}/
        ${CMAKE_CURRENT_LIST_DIR}/task/task.cpp
        ${CMAKE_CURRENT_LIST_DIR}/schedular/schedular.cpp
)
