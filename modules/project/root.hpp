/*
  root.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 2-Oct-2025
    
  The root element for a Project
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_project_root
#define H_project_root

#include "element.hpp"
#include "text.hpp"

class Element;

class ProjectRoot: public Element {

public:
  ProjectRoot(const std::string &name, std::vector<Element *> &objs);

  // Element
  virtual void setParent(Element *parent) { _parent = parent; }
  virtual Element *getParent() { return _parent; }
  virtual void build(Renderer &renderer);
  virtual Size layout();
  virtual void render(Renderer &renderer, const Point &origin);
  virtual Size size() { return _size; }

protected:

  Element *_parent;
  Size _size;
  Text _name;
  std::vector<std::unique_ptr<Element> > _objs;

};

#endif // H_project_root
