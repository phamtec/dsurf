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
class Point;
class TextEditor;

class Box {

public:
  Box() {};
  virtual ~Box() {};
  
  virtual Size layout() = 0;
    // layout the object.

  virtual void build(Renderer &renderer) {}
    // build the texture and surface ready to render.
    // objects, build yourself and then call super::build(renderer)
    
  virtual void render(Renderer &renderer, const Point &origin) {}
    // render this object to the screen.
    // objects, render yourself and then call super::render(renderer, origin)

  virtual Box *hitTest(const Point &origin, const Point &p);
    // recursivle find the box at the point.
    // objects, find the object in your sub objects
    // and then call super::hitTest(origin, p)
    
  virtual void edit(TextEditor *editor) {}
    // edit this object with the text editor.
    
  Point origin();
    // recursivle return the origin of this object.
    
  virtual Point localOrigin(int index) { return Point(); }
    // return the origin of an object with index in this object.

};

#endif // H_box
