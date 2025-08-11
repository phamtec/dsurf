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
  *offs += Size(d);
  
}

Point Spatial::center(float width, float height, const Size &size, float scale) {

  return {
    .x = ((width - (size.w * scale)) * (1/scale)) / 2, 
    .y = ((height - (size.h * scale)) * (1/scale)) / 2 
  };
    
}

void Spatial::scaleAndCenter(int width, int height, const Point &mouse, float amount, float prescale, float *scale, Size *offset) {

  float origscale = *scale;
  
  *scale += amount * prescale;
  if (*scale < 0.01) {
    *scale = 0.01;
  }
  
//   if (*scale >= 2.0 && *scale <= 2.1) {
//     *offset = Size(-138, -60);
//   }

  // normalise original offset
  Size offsn = *offset;
  offsn *= origscale;
  
  // distance from center.
  Point c(width/2, height/2);
  Size dist(mouse.x - c.x, mouse.y - c.y);
  
  // take out normalised distance
  dist -= offsn;
  
  // scale the distance.
  dist /= *scale;
  dist /= 2;
  *offset += dist;
  
//  cout << *offset << endl;
  
}
