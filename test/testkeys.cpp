/*
  testkeys.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 23-Sep-2025
    
  Basic fundamental tests on how keys are assigned
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <SDL3/SDL_events.h>

using namespace std;

BOOST_AUTO_TEST_CASE( numKeys )
{
  cout << "=== numKeys ===" << endl;

  // there is some code which relies on this.
  BOOST_CHECK_EQUAL(SDLK_9 - SDLK_1, 8);
  
}



