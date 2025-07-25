#   CMakeLists.txt
#
#   Author: Paul Hamilton (phamtec@mac.com)
#   Date: 25-Jul-2025
#
#   Input file for dead
#
#   Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
#   
#   https://github.com/phamtec/dead

cmake_minimum_required(VERSION 3.5)
cmake_policy(SET CMP0072 NEW) # OpenGL

project (dead)
  find_package(Boost 1.85.0 COMPONENTS program_options REQUIRED)
  find_package(SDL3 REQUIRED)
  find_package(SDL3_ttf REQUIRED)
  find_package(OpenGL)
  add_definitions(-DBOOST_ALL_DYN_LINK) 
  enable_testing()
  
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

if (UNIX AND NOT APPLE)
  add_definitions(-funwind-tables) 
endif ()

include_directories(include)

add_executable(dead src/dead.cpp)
    target_link_libraries(dead SDL3::SDL3 SDL3_ttf::SDL3_ttf OpenGL::GL ${Boost_PROGRAM_OPTIONS_LIBRARY})

# Tests
