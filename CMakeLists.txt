#   CMakeLists.txt
#
#   Author: Paul Hamilton (phamtec@mac.com)
#   Date: 25-Jul-2025
#
#   Input file for dsurf
#
#   Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
#   
#   https://github.com/phamtec/dead

cmake_minimum_required(VERSION 3.5)
cmake_policy(SET CMP0167 NEW) # Boost
cmake_policy(SET CMP0072 NEW) # OpenGL

project (dsurf)
  find_package(Boost 1.85.0 COMPONENTS program_options unit_test_framework REQUIRED)
  find_package(SDL3 REQUIRED)
  find_package(SDL3_ttf REQUIRED)
  find_package(OpenGL)
  add_definitions(-DBOOST_ALL_DYN_LINK) 
  enable_testing()
  
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)

if (UNIX AND NOT APPLE)
  add_definitions(-funwind-tables) 
endif ()

include_directories(include)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

set(SRC
  src/font.cpp 
  src/text.cpp 
  src/renderer.cpp 
  src/builder.cpp 
  src/stringprop.cpp
  src/string.cpp
  src/list.cpp 
  src/listprop.cpp
  src/dict.cpp
  src/dictprop.cpp
  src/colours.cpp
  src/sizes.cpp
  src/long.cpp
  src/longprop.cpp
  src/box.cpp
  src/spatial.cpp
  src/point.cpp
  src/size.cpp
  src/rect.cpp
  src/filledbox.cpp
  src/gfx.cpp
)

add_executable(dsurf src/dsurf.cpp ${SRC})
  target_link_libraries(dsurf SDL3::SDL3 SDL3_ttf::SDL3_ttf OpenGL::GL ${Boost_PROGRAM_OPTIONS_LIBRARY} reflectcpp)

# Tests
add_executable(TestSpatial test/testspatial.cpp src/spatial.cpp src/point.cpp src/size.cpp)
  target_link_libraries(TestSpatial ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(TestSpatial TestSpatial)

add_executable(TestBuilder test/testbuilder.cpp src/builder.cpp ${SRC})
  target_link_libraries(TestBuilder reflectcpp ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} SDL3::SDL3 SDL3_ttf::SDL3_ttf OpenGL::GL)

add_test(TestBuilder TestBuilder)
