/*
  box.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 6-Aug-2025
    
  Box class.
  
  Box is the base class of all drawn objects.
  
  Box
    Prop
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
class Font;
class Resources;
class Point;

class Box {

public:
  Box() {};
  virtual ~Box() {};
  
  virtual void build(Renderer &renderer, Font &font);
    // build the texture and surface ready to render.

  virtual Size layout(Resources &res, const Point &origin) = 0;
    // layout the object.

  virtual void render(Renderer &renderer, Resources &pool);
    // render this object to the screen.

protected:
  Rect _r;
};

#endif // H_box
