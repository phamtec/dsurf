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
  virtual void setParent(Element *parent) override  { _parent = parent; }
  virtual Element *getParent() override  { return _parent; }
  virtual void build(Renderer &renderer) override ;
  virtual void layout() override ;
  virtual void render(Renderer &renderer, const Point &origin) override ;
  virtual Element *hitTest(const Point &origin, const Point &p) override ;
  virtual Point localOrigin(Element *elem) override ;
  virtual void destroy(Renderer &renderer) override ;
  virtual Size size() override { return _size; }
  virtual bool visit(std::function<bool (Element *)> f) override;

  // Writeable
  virtual std::string getName() override ;
  virtual rfl::Generic getGeneric() override ;

  // Editable
  virtual std::wstring getString() override ;
  virtual void setString(Renderer &renderer, const std::wstring &s) override ;

  // Commandable
  virtual void initHUD(HUD *hud) override ;
  virtual void setMode(Renderer &renderer, HUD *hud) override ;
  static void registerHUDModes(HUD *hud);
  virtual void processKey(Renderer &renderer, SDL_Keycode code) override ;

  // Objable
  virtual void setObj(Renderer &renderer, Element *obj) override ;
  virtual Element *getObj() override  { return _obj.get(); }
    
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
