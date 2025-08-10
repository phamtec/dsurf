/*
  spatial.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 10-Aug-2025
    
  Spatial tools.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_spatial
#define H_spatial

#include <SDL3/SDL_rect.h>

class Spatial {

public:

  static inline SDL_FPoint makePoint(float x, float y) {
    // make a new point
    return { .x = x, .y = y };
  }
    
  static inline void add(SDL_FPoint *p, const SDL_FPoint &d) {
    // add 2 points together
    p->x += d.x;
    p->y += d.y;
  }
  
  static inline void subtract(SDL_FPoint *p, const SDL_FPoint &d) {
    // subtract one point from another.
    p->x -= d.x;
    p->y -= d.y;
  }

  static inline SDL_FPoint origin(const SDL_FRect &r) {
    // return the origin of a rectqngle
    return makePoint(r.x, r.y);
  }
  
  static inline void setOrigin(SDL_FRect *r, const SDL_FPoint &origin) {
    // set the origin of a rectangle
    r->x = origin.x;
    r->y = origin.y;
  }

  static inline void setDimensions(SDL_FRect *r, float width, float height) {
    // set the dimensions of a rectangle
    r->w = width;
    r->h = height;
  }

  static inline void addOrigin(SDL_FRect *r, const SDL_FPoint &d) {
    // add a distance to the origin of a rectangle
    r->x += d.x;
    r->y += d.y;
  }
  
  static inline void subtractOrigin(SDL_FRect *r, const SDL_FPoint &d) {
    // subtract a distance from the origin of a rectangle
    r->x -= d.x;
    r->y -= d.y;
  }
  
  static inline void inset(SDL_FRect *r, double v) {
    // inset a rectangle by an amount on all sides (shrink or grow)
    r->x += v;
    r->y += v;
    r->w -= v * 2;
    r->h -= v * 2;
  }

  static inline SDL_FPoint calcOriginOffset(const SDL_FRect &r, float width, float height) {
  // calculate the offset to the origin of a rectangle.
    return makePoint(r.x + width, r.y + height);
  }
  
  static void calcPan(const SDL_FPoint &p, SDL_FPoint *last, SDL_FPoint *offs);
    // calculate the pan. Takes the new point of a pan
    // last point of the pan, the offset to pan
    
};

#endif // H_spatial
