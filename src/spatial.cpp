/*
  spatial.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 10-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "spatial.hpp"

#include <iostream>

using namespace std;

void Spatial::calcPan(const Point &p, Point *last, Size *offs, float scale) {

  Size d = (*last - p) * (1/scale);
  *last = p;
  *offs -= d;
  
}

Point Spatial::center(const Size &wsize, const Size &size, float scale) {

//  cout << "Spatial::center (" << wsize << "), (" << size << "), " << scale << endl;
  return ((wsize - (size * scale)) * (1/scale)) / 2;
    
}

void Spatial::scaleAndCenter(const Size &objsize, float amount, float prescale, float *scale, Size *offset) {

//   cout << "scaleAndCenter " << amount << ", " << prescale << endl;
//   cout << "scale " << *scale << endl;
//   cout << "offset " << *offset << endl;

  float oscale = *scale;
   
  *scale += amount * prescale;
  if (*scale < 0.01) {
    *scale = 0.01;
  }
  
  Size newsize = objsize + (objsize * (*scale - oscale));
  Size diff = (newsize - objsize) * 2;
//  cout << "diff " << diff << endl;
  
  *offset -= diff * (1 / oscale);
  
}
