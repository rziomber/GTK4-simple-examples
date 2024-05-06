cmake_minimum_required (VERSION 3.15)

project(
  GTK-4_EXAMPLE
  VERSION 1.0
  LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-O2")
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk4)

include_directories(${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})

add_executable(main gtk4.cpp)
set_property(TARGET main PROPERTY CXX_STANDARD 23)

target_link_libraries(main ${CMAKE_DL_LIBS} ${GTK_LIBRARIES})
