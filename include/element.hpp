/*
  element.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 6-Aug-2025
    
  Element bass class.
  
  Element is the abstract base class of all drawn objects.
  
  Element
    String
    Bool
    Long
    List
    Dict
    Property
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_element
#define H_element

#include "rect.hpp"

class Renderer;
class Point;

class Element {

public:
  Element() {};
  virtual ~Element() {};
  
  virtual std::string describe();
    // describe this element.
    
  virtual Size layout() = 0;
    // layout the object.

  virtual void build(Renderer &renderer) = 0;
    // build the texture and surface ready to render.
    // objects, build yourself and then call super::build(renderer)
    
  virtual void destroy(Renderer &renderer) {};
    // opposite of build, but called with the renderer.
    
  virtual void render(Renderer &renderer, const Point &origin) = 0;
    // render this object to the screen.
    // objects, render yourself and then call super::render(renderer, origin)

  virtual Element *hitTest(const Point &origin, const Point &p);
    // recursivle find the element at the point.
    // objects, find the object in your sub objects
    // and then call super::hitTest(origin, p)
    
  Point origin();
    // recursivle return the origin of this object.
    
  virtual Point localOrigin(Element *elem) { return Point(); }
    // return the origin of an object in this object.
    
  Element *root();
    // return the root object by walking the parent.
    
};

#endif // H_element
