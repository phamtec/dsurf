/*
  obj.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 2-Oct-2025
    
  The object element for a Project
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_project_obj
#define H_project_obj

#include "element.hpp"
#include "text.hpp"
#include "commandable.hpp"

class Element;

class ProjectObj: public Element, public Commandable {

public:
  ProjectObj(const std::string &name, const std::string &filename);

  // Element
  virtual void setParent(Element *parent) { _parent = parent; }
  virtual Element *getParent() { return _parent; }
  virtual void build(Renderer &renderer);
  virtual Size layout();
  virtual void render(Renderer &renderer, const Point &origin);
  virtual Size size() { return _size; }

  // Commandable
  virtual void initHUD(HUD *hud);
  virtual void setMode(Renderer &renderer, HUD *hud);
  virtual void processKey(Renderer &renderer, SDL_Keycode code);

protected:

  Element *_parent;
  Size _size;
  Text _name;
  std::string _filename;
  int _hudobj;
  
};

#endif // H_project_obj
