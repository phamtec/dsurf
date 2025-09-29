/*
  listelem.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 13-Sep-2025
  
  A list element. When editing the list, this displays without the underlying
    content and is used to edit elements in the list.
    
  Otherwise it's just a pass through proxy to the underlying element in the list.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_listelem
#define H_listelem

#include "element.hpp"
#include "text.hpp"
#include "writeable.hpp"
#include "editable.hpp"
#include "commandable.hpp"
#include "objable.hpp"

#include <memory>
#include <vector>

class ListElem: public Element,  public Writeable, public Editable, public Commandable, public Objable  {

  typedef Element super;

public:
  ListElem(Element *obj);
  
  void setEdit(Renderer &renderer, bool state);
    // we are editing the list element.
    
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

  // Objable
  virtual void setObj(Renderer &renderer, Element *obj);
    
private:

  Element *_parent;
  Size _size;
  bool _editing;
  std::unique_ptr<Element> _obj;
  SDL_Texture *_texture;
  Size _textsize;
  int _hudlistelem;
  
};

#endif // H_listelem
