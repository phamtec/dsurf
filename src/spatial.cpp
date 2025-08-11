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

  Point d = (*last - p);
  d *= 1/scale;
  *last = p;
  *offs -= Size(d);
  
}

Point Spatial::center(const Size &wsize, const Size &size, float scale) {

//  cout << "Spatial::center (" << wsize << "), (" << size << "), " << scale << endl;
  
  return {
    .x = ((wsize.w - (size.w * scale)) * (1/scale)) / 2, 
    .y = ((wsize.h - (size.h * scale)) * (1/scale)) / 2 
  };
    
}

void Spatial::scaleAndCenter(const Size &wsize, const Size &osize, const Point &mouse, float amount, float prescale, float *scale, Size *offset) {

  cout << "scaleAndCenter " << wsize << ", " << mouse << ", " << amount << ", " << prescale << endl;
  cout << "scale " << *scale << endl;
  cout << "offset " << *offset << endl;

//   // Snap the area at the mouse to the center.
//   Point c(width/2, height/2);
//   Size dist(mouse.x - c.x, mouse.y - c.y);
//   dist *= 1 / *scale;
//   cout << dist << endl;
//   *offset -= dist;
  
  float oscale = *scale;
   
  *scale += amount * prescale;
  if (*scale < 0.01) {
    *scale = 0.01;
  }
  
  Size newsize = osize + (osize * (*scale - oscale));
  Size diff = newsize - osize;
  cout << diff << endl;
  
  *offset -= diff * (1 / *scale);

//  float sd =  *scale - origscale;
//  Size newoffs = 
//   float sd =  *scale - origscale;
//   Size dist = *offset;
//   dist *= sd;
//   cout << dist << endl;
//   *offset -= dist;
  
//   dist *= sd;
//   cout << dist << endl;  
//   *offset -= dist;
//  cout << dist << endl;  
//   
// //   if (*scale >= 2.0 && *scale <= 2.1) {
// //     *offset = Size(-138, -60);
// //   }
// 
//   // normalise original offset
//   Size offsn = *offset;
//   offsn *= origscale;
//   
//   // distance from center.
//   Point c(width/2, height/2);
//   Size dist(mouse.x - c.x, mouse.y - c.y);
//   
//   // take out normalised distance
//   dist -= offsn;
//   
//   // scale the distance.
//   dist /= *scale;
//   dist /= 2;
//   *offset += dist;
  
//  cout << *offset << endl;
  
}
