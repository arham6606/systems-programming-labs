cmake_minimum_required(VERSION 3.16)
project(app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    main.cpp
    physical_memory.cpp 
    page_table.cpp
)

add_executable(app ${SOURCES})

target_include_directories(app PRIVATE ${CMAKE_SOURCE_DIR})
