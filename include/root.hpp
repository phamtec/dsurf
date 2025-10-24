/*
  root.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 3-Oct-2025
  
  The root element.
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_root
#define H_root

#include "element.hpp"
#include "commandable.hpp"
#include "writeable.hpp"
#include "editable.hpp"
#include "locatable.hpp"
#include "text.hpp"

#include <memory>
#include <vector>

class Root: public Element, public Writeable, public Editable, public Commandable, public Locatable  {

  typedef Element super;

public:
  Root(const std::string &filename, Element *obj);
  
  Element *getObj() { return _obj.get(); }
  Text *getFilenameObj() { return &_filename; }
  
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

  // Writeable
  virtual std::string getName() override;
  virtual rfl::Generic getGeneric() override;
  virtual void setDirty(Renderer &renderer, bool state) override;
  virtual std::optional<std::string> getFilename() override;

  // Editable
  virtual std::wstring getString() override;
  virtual void setString(Renderer &renderer, const std::wstring &s) override;

  // Commandable
  virtual void initHUD(HUD *hud) override;
  virtual void setMode(Renderer &renderer, HUD *hud) override;
  static void registerHUDModes(HUD *hud);
  virtual void processKey(Renderer &renderer, SDL_Keycode code) override;

  // Locatable
  virtual Point getLocation() override { return _location; }
  virtual void setLocation(const Point &loc) override { _location = loc; }

private:

  Size _size;
  std::unique_ptr<Element> _obj;
  RectList _layout;
  Text _filename;
  Point _location;
  
};

#endif // H_listelem
