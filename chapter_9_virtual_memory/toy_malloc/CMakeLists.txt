cmake_minimum_required(VERSION 3.16)
project(app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files are now in the src/ directory
set(SOURCES
    src/main.cpp
)

add_executable(app ${SOURCES})

# Tell the compiler to look for .hpp files in the inc/ directory
target_include_directories(app PRIVATE 
    ${CMAKE_SOURCE_DIR}/inc
   
)