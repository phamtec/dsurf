#   CMakeLists.txt
#
#   Author: Paul Hamilton (phamtec@mac.com)
#   Date: 25-Jul-2025
#
#   Input file for dsurf
#
#   Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
#   
#   https://github.com/visualopsholdings/dsurf

cmake_minimum_required(VERSION 3.5)
cmake_policy(SET CMP0167 NEW) # Boost
cmake_policy(SET CMP0072 NEW) # OpenGL

project (dsurf)
  find_package(Boost 1.85.0 COMPONENTS program_options unit_test_framework log_setup log REQUIRED)
  find_package(SDL3 REQUIRED)
  find_package(SDL3_ttf REQUIRED)
  find_package(OpenGL)
  find_package(cppzmq REQUIRED)
  find_package(yaml-cpp REQUIRED)
  add_definitions(-DBOOST_ALL_DYN_LINK) 
  enable_testing()
  
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  flo
  GIT_REPOSITORY https://github.com/visualopsholdings/flo.git
  GIT_TAG main
)
FetchContent_MakeAvailable(flo)

if (UNIX AND NOT APPLE)
  add_definitions(-funwind-tables) 
endif ()
if (APPLE)
  if (CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "arm64")
    set(HOMEBREW_PATH "/opt/homebrew")
  elseif (CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "x86_64")
    set(HOMEBREW_PATH "/usr/local")
  else()
      message(STATUS "Unknown processor architecture: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
  endif()  
  include_directories(/usr/local/include ${HOMEBREW_PATH}/Cellar/zeromq/4.3.5_2/include)
  LINK_DIRECTORIES(${HOMEBREW_PATH}/Cellar/zeromq/4.3.5_2/lib)
endif ()

include_directories(include)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)
include_directories(build/_deps/flo-src/include)

set(BOOSTLIBS ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_LOG_SETUP_LIBRARY} ${Boost_LOG_LIBRARY})

set(LIBS zmq)

set(SRC
  src/font.cpp 
  src/text.cpp 
  src/core.cpp 
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
  src/flo.cpp
  src/zmq.cpp
  src/remotezmq.cpp
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
  src/macutil.cpp
  src/layout.cpp
)

set(OBJSRC
  src/element.cpp
  src/string.cpp
  src/list.cpp
  src/listelem.cpp
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
  src/locatable.cpp
)

set(MODSRC
  src/modules.cpp
  modules/project/main.cpp
  modules/project/root.cpp
  modules/project/fileobj.cpp
  modules/project/zmqobj.cpp
  modules/project/unkobj.cpp
  modules/project/code.cpp
  modules/code/main.cpp
  modules/code/root.cpp
  modules/code/scenario.cpp
)

add_executable(dsurf src/dsurf.cpp ${SRC} ${UTILSRC} ${OBJSRC} ${MODSRC})
  target_link_libraries(dsurf SDL3::SDL3 SDL3_ttf::SDL3_ttf OpenGL::GL FloLib
    ${BOOSTLIBS} reflectcpp yaml-cpp::yaml-cpp ${LIBS})

add_executable(Send test/send.cpp)
  target_link_libraries(Send ${BOOSTLIBS} reflectcpp ${LIBS})

add_executable(GenKeys src/genkeys.cpp)
  target_link_libraries(GenKeys ${BOOSTLIBS} ${LIBS})

# Tests

add_executable(TestSpatial test/testspatial.cpp src/spatial.cpp src/point.cpp src/size.cpp)
  target_link_libraries(TestSpatial ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(TestSpatial TestSpatial)

add_executable(TestBuilder test/testbuilder.cpp src/builder.cpp test/dummyobjs.cpp test/dummytext.cpp src/flo.cpp ${OBJSRC} ${UTILSRC} ${MODSRC})
  target_link_libraries(TestBuilder FloLib yaml-cpp::yaml-cpp ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${BOOSTLIBS})

add_test(TestBuilder TestBuilder)

add_executable(TestErrable test/testerrable.cpp test/dummyobjs.cpp test/dummytext.cpp modules/code/test/dummyobjs.cpp ${OBJSRC} ${UTILSRC})
  target_link_libraries(TestErrable FloLib yaml-cpp::yaml-cpp reflectcpp ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(TestErrable TestErrable)

add_executable(TestMove test/testmove.cpp test/dummyobjs.cpp test/dummytext.cpp modules/code/test/dummyobjs.cpp ${OBJSRC} ${UTILSRC})
  target_link_libraries(TestMove FloLib yaml-cpp::yaml-cpp reflectcpp ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(TestMove TestMove)

add_executable(TestList test/testlist.cpp test/dummyobjs.cpp test/dummytext.cpp modules/code/test/dummyobjs.cpp  ${OBJSRC} ${UTILSRC})
  target_link_libraries(TestList FloLib yaml-cpp::yaml-cpp reflectcpp ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(TestList TestList)

add_executable(TestMem test/testmem.cpp)
  target_link_libraries(TestMem ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(TestMem TestMem)

add_executable(TestChanges test/testchanges.cpp src/changes.cpp)
  target_link_libraries(TestChanges reflectcpp ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(TestChanges TestChanges)

add_executable(TestKeys test/testkeys.cpp)
  target_link_libraries(TestKeys ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(TestKeys TestKeys)

add_executable(TestProject modules/project/test/testproject.cpp src/builder.cpp test/dummyobjs.cpp test/dummytext.cpp src/flo.cpp ${OBJSRC} ${UTILSRC} ${MODSRC})
  target_link_libraries(TestProject FloLib yaml-cpp::yaml-cpp ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${BOOSTLIBS})

add_test(TestProject TestProject)
