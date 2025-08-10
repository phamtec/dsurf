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
  
  Point last = Point(21.1, 22.4);
  Size offs = Size(50.2, 40);
  
  Spatial::calcPan(Point(12.5, 21), &last, &offs);
  
  BOOST_CHECK_EQUAL(last.x, 12.5);
  BOOST_CHECK_EQUAL(last.y, 21);
  BOOST_CHECK(offs.w >= 58.8 && offs.w <= 58.9);
//  cout << offs.w << endl;
  BOOST_CHECK(offs.h >= 41.4 && offs.h <= 41.5);

}

BOOST_AUTO_TEST_CASE( center )
{
  cout << "=== center ===" << endl;
  
  Point p = Spatial::center(1024, 768, Size(300, 300), 0.5);

  BOOST_CHECK_EQUAL(p.x, 874);
  BOOST_CHECK_EQUAL(p.y, 618);

}
