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

void Spatial::scaleAndCenter(const Size &wsize, const Size &objsize, const Size &mouse, float amount, float scalemult, float *scale, Size *offset) {

//   cout << "scaleAndCenter wsize(" << wsize << "), objsize(" << objsize << "), mouse(" << mouse << ") amount " << amount << ", scalemult " << scalemult << endl;
//   cout << "scale " << *scale << endl;
//   cout << "offset (scaled) " << *offset << endl;

  // save away the original scale
  float oldscale = *scale;
  
  *scale += amount * scalemult;
  if (*scale < 0.01) {
    *scale = 0.01;
  }
//  cout << "newscale " << *scale << endl;
  
//   if (*scale == 2.0) {
//     cout << "test" << endl;
//     *offset -= Size(33.3, 33.3);
//     return;
//   }

  Size halfobj = objsize / 2;
//  cout << "halfobj " << halfobj << endl;
  
  Size halfw = wsize / 2;
//  cout << "halfw " << halfw << endl;
  
  Size objcenter = (halfw / *scale) - halfobj;
//  cout << "objcenter " << objcenter << endl;
  
  Size moffset = (mouse - halfw) / *scale;
//  cout << "moffset " << moffset << endl;
  
  Size oldmouse = mouse / oldscale;
//  cout << "oldmouse " << oldmouse << endl;
  
  *offset = objcenter + ((moffset + (halfobj - (oldmouse - *offset))));
  
}

float Spatial::calcScale(int key) {

  return ((((9-key) + 1) * 4.0) / 10.0)  - 0.3;
  
}
