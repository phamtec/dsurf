/*
  root.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 23-Oct-2025
    
  The object element for Code
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_code_root
#define H_code_root

#include "element.hpp"
#include "text.hpp"
#include "commandable.hpp"
#include "locatable.hpp"
#include "writeable.hpp"
#include "editable.hpp"
#include "flo.hpp"

#include <rfl.hpp>

class Element;

class CodeRoot: public Element, public Writeable, public Commandable, public Locatable, public Editable  {

  typedef Element super;

public:
  CodeRoot(const std::string &filename, const rfl::Generic &obj);

  void setScenario(Core &core, const rfl::Generic &scenario, int index);
  void run(Core &core);

  // Element
  virtual void setParent(Element *parent) override { _parent = parent; }
  virtual Element *getParent() override { return _parent; }
  virtual void build(Core &core) override;
  virtual void layout() override;
  virtual void render(Core &core, const Point &origin) override;
  virtual Size size() override { return _size; }
  virtual bool visit(std::function<bool (Element *)> f) override;
  virtual RectList calcLayout() override;
  virtual Point localOrigin(Element *elem) override;
  virtual Element *hitTest(const Point &origin, const Point &p) override;
  virtual void changed(Core &core, Element *obj) override;
  virtual bool isRoot() override { return true; }

  // Writeable
  virtual std::string getName() override;
  virtual rfl::Generic getGeneric() override;
  virtual void setDirty(Core &core, bool state) override;
  virtual std::optional<std::string> getFilename() override;

  // Commandable
  virtual void initHUD(HUD *hud) override;
  virtual void setMode(Core &core, HUD *hud) override;
  virtual void processKey(Core &core, SDL_Keycode code) override;

  // Locatable
  virtual Point getLocation() override { return _location; }
  virtual void setLocation(const Point &loc) override { _location = loc; }

  // Editable
  virtual std::wstring getString() override;
  virtual void setString(Core &core, const std::wstring &s) override;

protected:

  Element *_parent;
  Size _size;
  Text _librarylabel;
  std::unique_ptr<Element> _library;
  Text _scenarioslabel;
  std::vector<std::unique_ptr<Element> > _scenarios;
  std::unique_ptr<Element> _transform;
  std::unique_ptr<Element> _output;
  std::unique_ptr<Element> _scenario;
  int _scindex; // where the scenario came from in _scenarios
  RectList _layout;
  Text _filename;
  Point _location;
  bool _running;
  int _hudobj;
  std::unique_ptr<Flo> _flo;
  
  void rebuildScenarios(Core &core);

};

#endif // H_code_root
