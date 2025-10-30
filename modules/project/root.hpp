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
#include "locatable.hpp"
#include "writeable.hpp"

#include <vector>
#include <memory>

class Element;

class ProjectRoot: public Element, public Writeable, public Commandable, public Locatable {

  typedef Element super;

public:
  ProjectRoot(const std::string &name, const std::string &filename, std::vector<Element *> &objs);

  // Element
  virtual void setParent(Element *parent) override;
  virtual Element *getParent() override { return nullptr; }
  virtual void build(Renderer &renderer) override;
  virtual void layout() override;
  virtual void render(Renderer &renderer, const Point &origin) override;
  virtual Element *hitTest(const Point &origin, const Point &p) override;
  virtual Point localOrigin(Element *elem) override;
  virtual Size size() override { return _size; }
  virtual RectList calcLayout() override;
  virtual bool visit(std::function<bool (Element *)> f) override;
  virtual bool isRoot() override { return true; }

  // Writeable
  virtual std::string getName() override;
  virtual rfl::Generic getGeneric() override;
  virtual void setDirty(Renderer &renderer, bool state) override;
  virtual std::optional<std::string> getFilename() override;

  // Commandable
  virtual void initHUD(HUD *hud) override;
  virtual void setMode(Renderer &renderer, HUD *hud) override;
  virtual void processKey(Renderer &renderer, SDL_Keycode code) override;

  // Locatable
  virtual Point getLocation() override { return _location; }
  virtual void setLocation(const Point &loc) override { _location = loc; }

protected:

  Size _size;
  Text _name;
  std::vector<std::unique_ptr<Element> > _objs;
  RectList _layout;
  Text _filename;
  int _hudroot;
  Point _location;
  
};

#endif // H_project_root
