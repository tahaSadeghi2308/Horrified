cmake_minimum_required (VERSION 3.22)
set(CMAKE_BUILD_TYPE Debug)

project(Horrified
  LANGUAGES CXX
)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

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
    src/gui.cc
    src/gui/hero_phase_page.cc
    src/gui/move_page.cc
    src/gui/pickup_page.cc
    src/gui/guide_page.cc
    src/gui/advanced_page.cc
    src/gui/end_game_page.cc
    src/gui/welcome_page.cc
    src/gui/monster_page.cc
    src/gui/player_setup_page.cc
    src/gui/defeat_page.cc
    src/gui/perk_page.cc
    src/gui/hero_selection_page.cc
    src/gui/load_match.cc
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

link_directories(${RAYLIB_PATH}/build/raylib)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib GL m pthread dl rt X11)

# parts for packages
find_package(fmt REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
