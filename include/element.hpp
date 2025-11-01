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
#include "layout.hpp"

#include <vector>
#include <functional>

class Core;
class Point;

class Element {

public:
  Element() {};
  virtual ~Element() {};
  
  virtual std::string describe();
    // describe this element.
    
  virtual void setParent(Element *parent) {}
    // set the parent.
    
  virtual Element *getParent() { return nullptr; }
    // get the parent of this element.

  virtual void layout() = 0;
    // layout the objects.
    // you would work out the size of the elemebt probably and
    // then return that below.

  virtual Size size() = 0;
    // return the size of this element.
    
  virtual RectList calcLayout() { return RectList{}; }
    // calculate the layout as a set of rects.
    // there are 3 places where you need to iterate through objects
    // and work out where they are. When rendering, when getting the
    // local origin of an object and when hit testing an object.
    // in your layout, you could layout the sub objects, then 
    // implement this to return a list of rectangles that parralell that,
    // save that away and then use it when your hit testing etc.
    
  virtual void build(Core &core)  {};
    // build the texture and surface ready to render.
    
  virtual void destroy(Core &core) {};
    // opposite of build, but called with the core.
    
  virtual void render(Core &core, const Point &origin) = 0;
    // render this object to the screen.
    // objects, render yourself and then call super::render(core, origin)

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
    
  virtual bool visit(std::function<bool (Element *)> f);
    // visit all the sub elements of this element.
    
  virtual void changed(Core &core, Element *) {}
    // an element has changed.
    
  virtual bool isRoot() { return false; }
    // return true if this is a root. Used by the testing.
    
};

#endif // H_element
