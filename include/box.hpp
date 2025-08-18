/*
  box.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 6-Aug-2025
    
  Box bass class.
  
  Box is the abstract base class of all drawn objects.
  
  Box
    StringProp
      BoolProp
    LongProp
    ListProp
    DictProp
    String
      Bool
    Long
    List
    Dict
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_box
#define H_box

#include "rect.hpp"

class Renderer;
class Resources;
class Point;

#include <rfl.hpp>

class Box {

public:
  Box() {};
  virtual ~Box() {};
  
  virtual Size layout() = 0;
    // layout the object.

  virtual void build(Renderer &renderer) {};
    // build the texture and surface ready to render.

  virtual void render(Renderer &renderer, const Point &origin) {};
    // render this object to the screen.

  virtual Box *hitTest(const Point &origin, const Point &p);
    // find the box at the point.
    
  Size _size;
    // the size of this object
    
  virtual rfl::Generic getGeneric() = 0;
  virtual std::string getName() = 0;
  
};

#endif // H_box
