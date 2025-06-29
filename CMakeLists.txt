cmake_minimum_required (VERSION 3.22)
set(CMAKE_BUILD_TYPE Debug)

project(Horrified
  LANGUAGES CXX
)
# ------------------------------ Execution part ------------------------------------------------
# Include directories
include_directories(./include)

# Main executable
set(
  PROJECT_SOURCES
  main.cc
  src/utility.cc
  src/exceptions.cc
  src/Hero.cc
  src/location.cc
  src/system.cc
  src/villager.cc
  src/monster.cc
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# parts for packages
find_package(fmt REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
