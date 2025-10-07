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
#include "text.hpp"

#include <memory>
#include <vector>

class Root: public Element,  public Writeable, public Editable, public Commandable  {

  typedef Element super;

public:
  Root(const std::string &filename, Element *obj);
  
  Element *getObj() { return _obj.get(); }
  std::string getFilename();
  Text *getFilenameObj() { return &_filename; }
  void setDirty(Renderer &renderer, bool state);
  
  void setLocation(const Point &loc) { _location = loc; }
  Point getLocation() { return _location; }
  
  // Element
  virtual void setParent(Element *parent) { _parent = parent; }
  virtual Element *getParent() { return _parent; }
  virtual void build(Renderer &renderer);
  virtual Size layout();
  virtual void render(Renderer &renderer, const Point &origin);
  virtual Element *hitTest(const Point &origin, const Point &p);
  virtual Point localOrigin(Element *elem);
  virtual void destroy(Renderer &renderer);
  virtual Size size() { return _size; }

  // Writeable
  virtual std::string getName();
  virtual rfl::Generic getGeneric();

  // Editable
  virtual std::wstring getString();
  virtual void setString(Renderer &renderer, const std::wstring &s);

  // Commandable
  virtual void initHUD(HUD *hud);
  virtual void setMode(Renderer &renderer, HUD *hud);
  static void registerHUDModes(HUD *hud);
  virtual void processKey(Renderer &renderer, SDL_Keycode code);

private:

  Element *_parent;
  Size _size;
  std::unique_ptr<Element> _obj;
  Text _filename;
  Point _location;
  
};

#endif // H_listelem
