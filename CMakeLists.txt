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
  find_package(cppzmq REQUIRED)
  find_package(yaml-cpp REQUIRED)
  add_definitions(-DBOOST_ALL_DYN_LINK) 
  enable_testing()
  
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)

if (UNIX AND NOT APPLE)
  add_definitions(-funwind-tables) 
endif ()
if (APPLE)
  include_directories(/usr/local/include /opt/homebrew/Cellar/zeromq/4.3.5_2/include)
  LINK_DIRECTORIES(/opt/homebrew/Cellar/zeromq/4.3.5_2/lib)
endif ()

include_directories(include)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

set(LIBS zmq)

set(SRC
  src/font.cpp 
  src/text.cpp 
  src/renderer.cpp 
  src/builder.cpp 
  src/spatial.cpp
  src/filledbox.cpp
  src/gfx.cpp
  src/corner.cpp
  src/resources.cpp
  src/texteditor.cpp
  src/hud.cpp
  src/hudmode.cpp
  src/shortcut.cpp
  src/types.cpp
  src/test.cpp
  src/changes.cpp
)

set(UTILSRC
  src/colours.cpp
  src/sizes.cpp
  src/point.cpp
  src/size.cpp
  src/rect.cpp
  src/unicode.cpp
  src/err.cpp
  src/move.cpp
)

set(OBJSRC
  src/element.cpp
  src/string.cpp
  src/list.cpp
  src/listelem.cpp
  src/dict.cpp
  src/long.cpp
  src/bool.cpp
  src/property.cpp
  src/editable.cpp
  src/commandable.cpp
  src/listable.cpp
  src/writeable.cpp
  src/removefromlist.cpp
  src/newelement.cpp
  src/objable.cpp
  src/setobj.cpp
  src/root.cpp
)

set(MODSRC
  src/modules.cpp
  modules/project/main.cpp
  modules/project/root.cpp
  modules/project/obj.cpp
)

add_executable(dsurf src/dsurf.cpp ${SRC} ${UTILSRC} ${OBJSRC} ${MODSRC})
  target_link_libraries(dsurf SDL3::SDL3 SDL3_ttf::SDL3_ttf OpenGL::GL ${Boost_PROGRAM_OPTIONS_LIBRARY} reflectcpp yaml-cpp::yaml-cpp ${LIBS})

add_executable(Send test/send.cpp)
  target_link_libraries(Send ${Boost_PROGRAM_OPTIONS_LIBRARY} reflectcpp ${LIBS})

# Tests
add_executable(TestSpatial test/testspatial.cpp src/spatial.cpp src/point.cpp src/size.cpp)
  target_link_libraries(TestSpatial ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(TestSpatial TestSpatial)

add_executable(TestBuilder test/testbuilder.cpp src/builder.cpp test/dummyobjs.cpp ${OBJSRC} ${UTILSRC} ${MODSRC})
  target_link_libraries(TestBuilder reflectcpp yaml-cpp::yaml-cpp ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(TestBuilder TestBuilder)

add_executable(TestErrable test/testerrable.cpp test/dummyobjs.cpp ${OBJSRC} ${UTILSRC})
  target_link_libraries(TestErrable reflectcpp ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(TestErrable TestErrable)

add_executable(TestMove test/testmove.cpp test/dummyobjs.cpp ${OBJSRC} ${UTILSRC})
  target_link_libraries(TestMove reflectcpp ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(TestMove TestMove)

add_executable(TestList test/testlist.cpp test/dummyobjs.cpp ${OBJSRC} ${UTILSRC})
  target_link_libraries(TestList reflectcpp ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(TestList TestList)

add_executable(TestMem test/testmem.cpp)
  target_link_libraries(TestMem ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(TestMem TestMem)

add_executable(TestChanges test/testchanges.cpp src/changes.cpp)
  target_link_libraries(TestChanges ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(TestChanges TestChanges)

