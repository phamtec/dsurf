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
include_directories(flo/include)

set(BOOSTLIBS ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_LOG_SETUP_LIBRARY} ${Boost_LOG_LIBRARY})

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
  src/zmq.cpp
  src/flo.cpp
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
  src/generic.cpp
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
)

set(FLOSRC
  flo/src/functions.cpp
  flo/src/processor.cpp
  flo/src/state.cpp
  flo/src/transform.cpp
  flo/src/functions/dict.cpp
  flo/src/functions/equal.cpp
  flo/src/functions/getmember.cpp
  flo/src/functions/if.cpp
  flo/src/functions/null.cpp
  flo/src/functions/select.cpp
  flo/src/functions/setmember.cpp
  flo/src/functions/string.cpp
  flo/src/functions/true.cpp
  flo/src/functions/apply.cpp
  flo/src/functions/cur.cpp
  flo/src/functions/pass.cpp
)

add_executable(dsurf src/dsurf.cpp ${SRC} ${UTILSRC} ${OBJSRC} ${MODSRC} ${FLOSRC})
  target_link_libraries(dsurf SDL3::SDL3 SDL3_ttf::SDL3_ttf OpenGL::GL 
    ${BOOSTLIBS} reflectcpp yaml-cpp::yaml-cpp ${LIBS})

add_executable(Send test/send.cpp)
  target_link_libraries(Send ${BOOSTLIBS} reflectcpp ${LIBS})

add_executable(GenKeys src/genkeys.cpp)
  target_link_libraries(GenKeys ${BOOSTLIBS} ${LIBS})

if (APPLE)
  set(SDL_XCFRAMEWORK_PATH /Users/paul/Dev/SDL/Xcode/SDL/build/SDL3.xcframework)
  set(SDL_TTF_XCFRAMEWORK_PATH /Users/paul/Dev/SDL_ttf/Xcode/build/SDL3_ttf.xcframework)
  add_executable(DSurfer MACOSX_BUNDLE src/dsurf.cpp ${SRC} ${UTILSRC} ${OBJSRC} ${MODSRC} ${FLOSRC} fonts/Monaco.ttf)
  target_link_libraries(DSurfer
      "-framework Cocoa"
      "-framework Foundation"
      "-framework OpenGL"
      ${SDL_XCFRAMEWORK_PATH}
      ${SDL_TTF_XCFRAMEWORK_PATH}
      ${BOOSTLIBS} reflectcpp yaml-cpp::yaml-cpp ${LIBS}
  )
  target_compile_definitions(DSurfer PRIVATE MAC_APP)
  set_target_properties(DSurfer PROPERTIES
    BUNDLE TRUE
    RESOURCE "../fonts/Monaco.ttf"
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.visualops.dsurfer"
    MACOSX_BUNDLE_BUNDLE_NAME "DSurfer"
    MACOSX_BUNDLE_BUNDLE_VERSION "0.9.0"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "0.9"
  )
  set_source_files_properties(fonts/Monaco.ttf
    PROPERTIES
    MACOSX_PACKAGE_LOCATION "Resources"
  )
  add_custom_command(
    TARGET DSurfer
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL_XCFRAMEWORK_PATH}/macos-arm64_x86_64/SDL3.framework DSurfer.app/Contents/Frameworks/SDL3.framework
  )
  add_custom_command(
    TARGET DSurfer
    POST_BUILD
    COMMAND install_name_tool -change @rpath/SDL3.framework/Versions/A/SDL3 @executable_path/../Frameworks/SDL3.framework/Versions/A/SDL3 DSurfer.app/Contents/MacOS/DSurfer
  )
  add_custom_command(
    TARGET DSurfer
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL_TTF_XCFRAMEWORK_PATH}/macos-arm64_x86_64/SDL3_ttf.framework DSurfer.app/Contents/Frameworks/SDL3_ttf.framework
  )
  add_custom_command(
    TARGET DSurfer
    POST_BUILD
    COMMAND install_name_tool -change @rpath/SDL3_ttf.framework/Versions/A/SDL3_ttf @executable_path/../Frameworks/SDL3_ttf.framework/Versions/A/SDL3_ttf DSurfer.app/Contents/MacOS/DSurfer
  )
  add_custom_command(
    TARGET DSurfer
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy /usr/local/lib/libboost_program_options.dylib DSurfer.app/Contents/Frameworks/
  )
  add_custom_command(
    TARGET DSurfer
    POST_BUILD
    COMMAND install_name_tool -change @rpath/libboost_program_options.dylib @executable_path/../Frameworks/libboost_program_options.dylib DSurfer.app/Contents/MacOS/DSurfer
  )
  add_custom_command(
    TARGET DSurfer
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy /opt/homebrew/opt/zeromq/lib/libzmq.5.dylib DSurfer.app/Contents/Frameworks/
  )
  add_custom_command(
    TARGET DSurfer
    POST_BUILD
    COMMAND install_name_tool -change /opt/homebrew/opt/zeromq/lib/libzmq.5.dylib @executable_path/../Frameworks/libzmq.5.dylib DSurfer.app/Contents/MacOS/DSurfer
  )
endif ()

# Tests

add_executable(TestSpatial test/testspatial.cpp src/spatial.cpp src/point.cpp src/size.cpp)
  target_link_libraries(TestSpatial ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(TestSpatial TestSpatial)

add_executable(TestBuilder test/testbuilder.cpp src/builder.cpp test/dummyobjs.cpp test/dummytext.cpp src/generic.cpp src/flo.cpp ${OBJSRC} ${UTILSRC} ${MODSRC} ${FLOSRC})
  target_link_libraries(TestBuilder reflectcpp yaml-cpp::yaml-cpp ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${BOOSTLIBS})

add_test(TestBuilder TestBuilder)

add_executable(TestErrable test/testerrable.cpp test/dummyobjs.cpp test/dummytext.cpp ${OBJSRC} ${UTILSRC})
  target_link_libraries(TestErrable reflectcpp ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(TestErrable TestErrable)

add_executable(TestMove test/testmove.cpp test/dummyobjs.cpp test/dummytext.cpp ${OBJSRC} ${UTILSRC})
  target_link_libraries(TestMove reflectcpp ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(TestMove TestMove)

add_executable(TestList test/testlist.cpp test/dummyobjs.cpp test/dummytext.cpp ${OBJSRC} ${UTILSRC})
  target_link_libraries(TestList reflectcpp ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(TestList TestList)

add_executable(TestMem test/testmem.cpp)
  target_link_libraries(TestMem ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(TestMem TestMem)

add_executable(TestChanges test/testchanges.cpp src/changes.cpp)
  target_link_libraries(TestChanges ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(TestChanges TestChanges)

add_executable(TestKeys test/testkeys.cpp)
  target_link_libraries(TestKeys ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(TestKeys TestKeys)

add_executable(TestFlo test/testflo.cpp src/flo.cpp src/generic.cpp ${FLOSRC})
  target_link_libraries(TestFlo reflectcpp ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${Boost_LOG_LIBRARY})

add_test(TestFlo TestFlo)

add_executable(TestFloSimple flo/test/simpletest.cpp src/flo.cpp src/generic.cpp ${FLOSRC})
  target_link_libraries(TestFloSimple reflectcpp ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${Boost_LOG_LIBRARY})

add_test(TestFloSimple TestFloSimple)

add_executable(TestFloHello flo/test/helloworldtest.cpp src/flo.cpp src/generic.cpp ${FLOSRC})
  target_link_libraries(TestFloHello reflectcpp ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${Boost_LOG_LIBRARY})

add_test(TestFloHello TestFloHello)

add_executable(TestFloLib flo/test/libtest.cpp src/flo.cpp src/generic.cpp ${FLOSRC})
  target_link_libraries(TestFloLib reflectcpp ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${Boost_LOG_LIBRARY})

add_test(TestFloLib TestFloLib)

add_executable(TestFloApply flo/test/applytest.cpp src/flo.cpp src/generic.cpp ${FLOSRC})
  target_link_libraries(TestFloApply reflectcpp ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${Boost_LOG_LIBRARY})

add_test(TestFloApply TestFloApply)
