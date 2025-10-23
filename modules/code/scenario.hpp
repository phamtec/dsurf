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

#include <rfl.hpp>

class Element;

class CodeScenario: public Element, public Commandable {

public:
  CodeScenario(const rfl::Generic &scenario);

  // Element
  virtual void setParent(Element *parent) override { _parent = parent; }
  virtual Element *getParent() override { return _parent; }
  virtual void build(Renderer &renderer) override;
  virtual void layout() override;
  virtual bool visit(std::function<bool (Element *)> f) override;
  virtual void render(Renderer &renderer, const Point &origin) override;
  virtual Size size() override { return _size; }
  virtual RectList calcLayout() override;

  // Commandable
  virtual void initHUD(HUD *hud) override;
  virtual void setMode(Renderer &renderer, HUD *hud) override;
  virtual void processKey(Renderer &renderer, SDL_Keycode code) override;

protected:

  Element *_parent;
  Size _size;
  Text _name;
  int _hudobj;
  int _hudedit;
  rfl::Generic _input;
  std::unique_ptr<Element> _editor;
  RectList _layout;
  bool _editing;
  
};

#endif // H_code_scenario
