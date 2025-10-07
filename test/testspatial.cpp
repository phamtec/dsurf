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
  
  Spatial::calcPan(Point(12.5, 21), &last, &offs, 1.0);
  
  BOOST_CHECK_EQUAL(last.x, 12.5);
  BOOST_CHECK_EQUAL(last.y, 21);
  BOOST_CHECK_CLOSE_FRACTION(offs.w, 41.5, 0.1);
//  cout << offs.w << endl;
  BOOST_CHECK_CLOSE_FRACTION(offs.h, 41.4, 0.1);

}

BOOST_AUTO_TEST_CASE( center )
{
  cout << "=== center ===" << endl;
  
  Point p = Spatial::center(Size(1024, 768), Size(300, 300), 0.5);

  BOOST_CHECK_EQUAL(p.x, 874);
  BOOST_CHECK_EQUAL(p.y, 618);

}

void test(const Size &wsize, const Size &osize, const Point &mouse, float amount, float prescale, float *scale, Size *offset) {

  float oscale = *scale;
  
  *scale += amount * prescale;
  if (*scale < 0.01) {
    *scale = 0.01;
  }
  
  Size newsize = osize + (osize * (*scale - oscale));
  Size diff = newsize - osize;
  cout << diff << endl;
  
  *offset -= diff;

}

BOOST_AUTO_TEST_CASE( scaleAndCenter )
{
  cout << "=== scaleAndCenter ===" << endl;

  Size wsize(500, 500);
  Size osize(200, 200);
  float scale = 1.0;
  Size offs(150.0, 150.0);
  Spatial::scaleAndCenter(wsize, osize, wsize / 2, 0.5, 1.0, &scale, &offs);
  
  BOOST_CHECK_CLOSE_FRACTION(scale, 1.5, 0.001);
  
  BOOST_CHECK_CLOSE_FRACTION(offs.w, 66.6, 0.1);
  BOOST_CHECK_CLOSE_FRACTION(offs.h, 66.6, 0.1);

  wsize = Size(1492, 862);
  osize = Size(936, 720);
  
  // values when test/list.json is shown.
  scale = 0.3;
  offs = Size(2018.67, 1076.67);
  // mouse slightly off center
  Point mouse =  (wsize / 2) - 10;
  Spatial::scaleAndCenter(wsize, osize, mouse, 0.5, 0.01, &scale, &offs);
  
  BOOST_CHECK_CLOSE_FRACTION(scale, .305, 0.001);
  BOOST_CHECK_CLOSE_FRACTION(offs.w, 1987.4, 0.1);
  BOOST_CHECK_CLOSE_FRACTION(offs.h, 1052.6, 0.1);
  
  scale = 1.0;
  offs = Size(0.0, 0.0);
  Spatial::scaleAndCenter(wsize, osize, mouse, 0.5, 1.0, &scale, &offs);
  
  BOOST_CHECK_EQUAL(scale, 1.5);
  BOOST_CHECK_CLOSE_FRACTION(offs.w, -245.3, 0.1);
  BOOST_CHECK_CLOSE_FRACTION(offs.h, -140.3, 0.1);
  
  scale = 1.5;
  offs = Size(-64.7, -68);
  Spatial::scaleAndCenter(wsize, osize, mouse, 0.5, 1.0, &scale, &offs);
  
  BOOST_CHECK_EQUAL(scale, 2);
  BOOST_CHECK_CLOSE_FRACTION(offs.w, -187.3, 0.1);
  BOOST_CHECK_CLOSE_FRACTION(offs.h, -144.5, 0.1);
  
}

BOOST_AUTO_TEST_CASE( calcScale )
{
  cout << "=== calcScale ===" << endl;

  BOOST_CHECK_CLOSE_FRACTION(Spatial::calcScale(1), 3.6, 0.1);
  BOOST_CHECK_CLOSE_FRACTION(Spatial::calcScale(2), 2.9, 0.1);
  BOOST_CHECK_CLOSE_FRACTION(Spatial::calcScale(3), 2.5, 0.1);
  BOOST_CHECK_CLOSE_FRACTION(Spatial::calcScale(4), 2.0, 0.1);
  BOOST_CHECK_CLOSE_FRACTION(Spatial::calcScale(5), 1.7, 0.1);
  BOOST_CHECK_CLOSE_FRACTION(Spatial::calcScale(6), 1.3, 0.1);
  BOOST_CHECK_CLOSE_FRACTION(Spatial::calcScale(7), 0.9, 0.1);
  BOOST_CHECK_CLOSE_FRACTION(Spatial::calcScale(8), 0.5, 0.1);
  BOOST_CHECK_CLOSE_FRACTION(Spatial::calcScale(9), 0.1, 0.1);

}
