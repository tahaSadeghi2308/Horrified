cmake_minimum_required (VERSION 3.22)

project(Horrified
  LANGUAGES CXX
)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ------------------------ Fetch FTXUI ------------------------------------
# include(FetchContent)

# set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
# set(FETCHCONTENT_QUIET OFF)

# FetchContent_Declare(ftxui
#   GIT_REPOSITORY https://github.com/arthursonzogni/ftxui.git
#   GIT_TAG        v5.0.0
#   GIT_PROGRESS   TRUE
#   GIT_SHALLOW    TRUE
#   EXCLUDE_FROM_ALL
# )
# FetchContent_MakeAvailable(ftxui)

# ------------------------------ Execution part ------------------------------------------------

# Enable testing
enable_testing()

# Find GTest package
find_package(GTest REQUIRED)

# Include directories
include_directories(./include)

# Main executable
set(
  PROJECT_SOURCES
  main.cc
  src/utility.cc
  src/item_bag.cc
  src/exceptions.cc
  src/perk_deck.cc
  src/monster_card_deck.cc
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# ========================= Test executable =====================
add_executable(deck_test test/deck_test.cc
  src/item_bag.cc
  src/exceptions.cc
  src/perk_deck.cc
  src/monster_card_deck.cc
)

# Link test executable with GTest
target_link_libraries(deck_test GTest::GTest GTest::Main)

# Add test
add_test(NAME deck_test COMMAND deck_test)

# target_link_libraries(ftxui-starter
#   PRIVATE ftxui::screen
#   PRIVATE ftxui::dom
#   PRIVATE ftxui::component  # Not needed for this example.
# )

# if (EMSCRIPTEN)
#   string(APPEND CMAKE_CXX_FLAGS " -s USE_PTHREADS")
#   string(APPEND CMAKE_EXE_LINKER_FLAGS " -s ASYNCIFY")
#   string(APPEND CMAKE_EXE_LINKER_FLAGS " -s PROXY_TO_PTHREAD")
# endif()
