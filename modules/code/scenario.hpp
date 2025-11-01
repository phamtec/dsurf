/*
  scenario.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 23-Oct-2025
    
  The object element for a Project
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_code_scenario
#define H_code_scenario

#include "element.hpp"
#include "text.hpp"
#include "commandable.hpp"
#include "writeable.hpp"

#include <rfl.hpp>

class Element;

class CodeScenario: public Element, public Writeable, public Commandable {

public:
  CodeScenario(const rfl::Generic &scenario, int index);

  // Element
  virtual void setParent(Element *parent) override { _parent = parent; }
  virtual Element *getParent() override { return _parent; }
  virtual void build(Core &core) override;
  virtual void layout() override;
  virtual void render(Core &core, const Point &origin) override;
  virtual Size size() override { return _size; }
  virtual RectList calcLayout() override;

  // Writeable
  virtual rfl::Generic getGeneric() override { return _scenario; }

  // Commandable
  virtual void initHUD(HUD *hud) override;
  virtual void setMode(Core &core, HUD *hud) override;
  virtual void processKey(Core &core, SDL_Keycode code) override;

protected:

  Element *_parent;
  Size _size;
  Text _name;
  int _hudobj;
  rfl::Generic _scenario;
  RectList _layout;
  bool _loaded;
  int _index;
  
};

#endif // H_code_scenario
