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
#include "commandable.hpp"
#include "text.hpp"
#include "flo.hpp"
#include "writeable.hpp"

#include <rfl.hpp>

class ProjectCode: public Element, public Writeable, public Commandable {

  typedef Element super;

public:
  ProjectCode(const std::string &name, rfl::Generic transform, std::optional<std::vector<rfl::Generic> > library, std::optional<rfl::Object<rfl::Generic> > scenario);

  void libChanged(Core &core, const std::vector<rfl::Generic> &library);
  
  // Element
  virtual void setParent(Element *parent) override { _parent = parent; }
  virtual Element *getParent() override { return _parent; }
  virtual void build(Core &core) override;
  virtual void layout() override;
  virtual void render(Core &core, const Point &origin) override;
  virtual Size size() override { return _size; }
  virtual RectList calcLayout() override;
  virtual Point localOrigin(Element *elem) override;
  virtual Element *hitTest(const Point &origin, const Point &p) override;
  virtual void changed(Core &core, Element *obj) override;
  virtual bool visit(std::function<bool (Element *)> f) override;

  // Writeable
  virtual std::string getName() override;
  virtual rfl::Generic getGeneric() override;

  // Commandable
  virtual void initHUD(HUD *hud) override;
  virtual void setMode(Core &core, HUD *hud) override;
  virtual void processKey(Core &core, SDL_Keycode code) override;

protected:

  Element *_parent;
  Size _size;
  Text _name;
  std::unique_ptr<Element> _transform;
  std::unique_ptr<Element> _input;
  std::unique_ptr<Element> _output;
  RectList _layout;
  int _hudobj;
  bool _running;
  std::unique_ptr<Flo> _flo;
    
  void run(Core &core);

};

#endif // H_project_code
