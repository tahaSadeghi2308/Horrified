cmake_minimum_required (VERSION 3.22)

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
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
