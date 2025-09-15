/*
  listelem.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 13-Sep-2025
  
  A list element, used while editing.  
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_listelem
#define H_listelem

#include "element.hpp"
#include "text.hpp"
#include "parentable.hpp"
#include "sizeable.hpp"
#include "writeable.hpp"
#include "editable.hpp"
#include "hudable.hpp"
#include "keyable.hpp"

#include <memory>
#include <vector>

class ListElem: public Element, public Parentable, public Sizeable, public Writeable, public Editable, public HUDable, public Keyable  {

  typedef Element super;

public:
  ListElem(Element *obj);
  
  void setEdit(Renderer &renderer, bool state);
  
  // Element
  virtual void build(Renderer &renderer);
  virtual Size layout();
  virtual void render(Renderer &renderer, const Point &origin);
  virtual Element *hitTest(const Point &origin, const Point &p);
  virtual Point localOrigin(int index);
  virtual void destroy(Renderer &renderer);

  // Writeable
  virtual std::string getName();
  virtual rfl::Generic getGeneric();

  // Parentable
  virtual void setParent(Element *parent, int index) { _parent = parent; _index = index; }
  virtual Element *getParent() { return _parent; }
  virtual int getIndex() { return _index; }
  
  // Sizeable
  virtual Size getSize() { return _size; }

  // Editable
  virtual std::wstring getString();
  virtual void setString(Renderer &renderer, const std::wstring &s);

  // HUDable
  virtual void initHUD(HUD *hud);
  virtual void setMode(Renderer &renderer, HUD *hud);

  // Keyable
  virtual void processKey(Renderer &renderer, SDL_Keycode code);

private:

  Element *_parent;
  int _index;
  Size _size;
  bool _editing;
  std::unique_ptr<Element> _obj;
  SDL_Texture *_texture;
  Size _textsize;
  
};

#endif // H_listelem
