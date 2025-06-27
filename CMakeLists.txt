cmake_minimum_required(VERSION 4.0)
project(Horrified)
set(CMAKE_BUILD_TYPE Debug)

set(
    PROJECT_SOURCE
    src/system.cc
    src/exceptions.cc
    src/place.cc
    src/monster.cc
    src/hero.cc
    src/tui.cc
    src/utility.cc
    main.cc
)

include_directories(./include)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE})

# parts for packages 
find_package(fmt REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)
