/*
  zmqobj.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 10-Oct-2025
    
  The object element for a Project
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_project_zmqobj
#define H_project_zmqobj

#include "element.hpp"
#include "text.hpp"
#include "commandable.hpp"
#include "flo.hpp"
#include "writeable.hpp"

#include <rfl.hpp>

class Element;

class ProjectZMQObj: public Element, public Writeable, public Commandable {

  typedef Element super;

public:
  ProjectZMQObj(const std::string &name, const rfl::Object<rfl::Generic> &obj);

  // Element
  virtual void setParent(Element *parent) override { _parent = parent; }
  virtual Element *getParent() override { return _parent; }
  virtual void build(Renderer &renderer) override;
  virtual void layout() override;
  virtual void render(Renderer &renderer, const Point &origin) override;
  virtual Element *hitTest(const Point &origin, const Point &p) override;
  virtual Size size() override { return _size; }
  virtual RectList calcLayout() override;
  virtual Point localOrigin(Element *elem) override;
  virtual bool visit(std::function<bool (Element *)> f) override;
  virtual void changed(Renderer &renderer, Element *obj) override;

  // Writeable
  virtual rfl::Generic getGeneric()  override;

  // Commandable
  virtual void initHUD(HUD *hud) override;
  virtual void setMode(Renderer &renderer, HUD *hud) override;
  virtual void processKey(Renderer &renderer, SDL_Keycode code) override;
  virtual std::string getVerb() override;

protected:

  Element *_parent;
  Size _size;
  Text _name;
  int _hudobj;
  bool _editing;
  std::vector<std::unique_ptr<Element> > _code;
  RectList _layout;
  int _libindex;
  
  std::string _remoteAddress;
  int _remotePort;
  std::string _remotePubKey;
  std::string _uuid;
  std::string _privateKey;
  std::string _publicKey;
  rfl::Object<rfl::Generic> _send;
  rfl::Object<rfl::Generic> _next;
  std::shared_ptr<Flo> _flo;

  void load(Renderer &renderer);
  std::optional<rfl::Object<rfl::Generic> > findScenario(std::optional<std::vector<rfl::Generic> > scenarios, const std::string &path);
  
};

#endif // H_project_zmqobj
