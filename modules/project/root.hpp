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
#include "commandable.hpp"

class Element;

class ProjectRoot: public Element, public Commandable {

  typedef Element super;

public:
  ProjectRoot(const std::string &name, const std::string &filename, std::vector<Element *> &objs);

  // Element
  virtual void setParent(Element *parent) { _parent = parent; }
  virtual Element *getParent() { return _parent; }
  virtual void build(Renderer &renderer);
  virtual Size layout();
  virtual void render(Renderer &renderer, const Point &origin);
  virtual Element *hitTest(const Point &origin, const Point &p);
  virtual Point localOrigin(Element *elem);
  virtual Size size() { return _size; }

  // Commandable
  virtual void initHUD(HUD *hud);
  virtual void setMode(Renderer &renderer, HUD *hud);
  virtual void processKey(Renderer &renderer, SDL_Keycode code);

protected:

  Element *_parent;
  Size _size;
  Text _name;
  std::vector<std::unique_ptr<Element> > _objs;
  std::string _filename;
  int _hudroot;
  
};

#endif // H_project_root
