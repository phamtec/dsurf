/*
  testspatial.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 10-Aug-2025
    
  Spatial tests.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "spatial.hpp"

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_CASE( calcPan )
{
  cout << "=== calcPan ===" << endl;
  
  SDL_FPoint last = Spatial::makePoint(21.1, 22.4);
  SDL_FPoint offs = Spatial::makePoint(50.2, 40);
  
  Spatial::calcPan(Spatial::makePoint(12.5, 21), &last, &offs);
  
  BOOST_CHECK_EQUAL(last.x, 12.5);
  BOOST_CHECK_EQUAL(last.y, 21);
  BOOST_CHECK(offs.x >= 58.8 && offs.x <= 58.9);
//  cout << offs.x << endl;
  BOOST_CHECK(offs.y >= 41.4 && offs.y <= 41.5);

}
