cmake_minimum_required(VERSION 4.0)
project(Horrified)

set(
    PROJECT_SOURCE
    src/system.cc
    src/exceptions.cc
    src/place.cc
    main.cc
)
include_directories(./include)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE})
