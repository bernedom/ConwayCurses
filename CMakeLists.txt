cmake_minimum_required(VERSION 3.21)

project(
  Conway
  LANGUAGES CXX
  DESCRIPTION "Conways Game of Life using curses"
  VERSION 1.0.0)

include(CTest)

find_package(Curses REQUIRED)

add_executable(conway src/main.cpp)
target_compile_features(conway PRIVATE cxx_std_20)
target_include_directories(conway PRIVATE ${CURSES_INCLUDE_DIR})
target_link_libraries(conway PRIVATE ${CURSES_LIBRARIES})

