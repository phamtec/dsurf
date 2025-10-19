/*
  code.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 19-Oct-2025
    
  The code element for a Project
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_project_code
#define H_project_code

#include "element.hpp"
#include "text.hpp"

#include <rfl.hpp>

class ProjectCode: public Element {

public:
  ProjectCode(const std::string &name, rfl::Generic source);

  // Element
  virtual void setParent(Element *parent) override { _parent = parent; }
  virtual Element *getParent() override { return _parent; }
  virtual void build(Renderer &renderer) override;
  virtual void destroy(Renderer &renderer) override;
  virtual void layout() override;
  virtual void render(Renderer &renderer, const Point &origin) override;
  virtual Size size() override { return _size; }
  virtual RectList calcLayout() override;

protected:

  Element *_parent;
  Size _size;
  Text _name;
  std::unique_ptr<Element> _obj;
  RectList _layout;
  rfl::Generic _source;
  
};

#endif // H_project_code
