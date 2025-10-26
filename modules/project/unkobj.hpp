/*
  unkobj.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 10-Oct-2025
    
  The object element for a Project
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_project_unkobj
#define H_project_unkobj

#include "element.hpp"
#include "text.hpp"
#include "commandable.hpp"
#include "writeable.hpp"

class Element;

class ProjectUnknownObj: public Element, public Writeable, public Commandable {

public:
  ProjectUnknownObj(const std::string &name, rfl::Generic g);

  // Element
  virtual void setParent(Element *parent) override { _parent = parent; }
  virtual Element *getParent() override { return _parent; }
  virtual void build(Renderer &renderer) override;
  virtual void layout() override;
  virtual void render(Renderer &renderer, const Point &origin) override;
  virtual Size size() override { return _size; }

  // Writeable
  virtual rfl::Generic getGeneric() override;

  // Commandable
  virtual void initHUD(HUD *hud) override;
  virtual void setMode(Renderer &renderer, HUD *hud) override;
  virtual void processKey(Renderer &renderer, SDL_Keycode code) override;

protected:

  Element *_parent;
  Size _size;
  Text _name;
  int _hudobj;
  rfl::Generic _g;
  
};

#endif // H_project_unkobj
