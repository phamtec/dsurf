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
#include "dict.hpp"

using vops::DictG;
using vops::DictO;
using vops::DictV;

class Element;

class ProjectZMQObj: public Element, public Writeable, public Commandable {

  typedef Element super;

public:
  ProjectZMQObj(const std::string &name, const DictO &obj);

  // Element
  virtual void setParent(Element *parent) override { _parent = parent; }
  virtual Element *getParent() override { return _parent; }
  virtual void build(Core &core) override;
  virtual void layout() override;
  virtual void render(Core &core, const Point &origin) override;
  virtual Element *hitTest(const Point &origin, const Point &p) override;
  virtual Size size() override { return _size; }
  virtual RectList calcLayout() override;
  virtual Point localOrigin(Element *elem) override;
  virtual bool visit(std::function<bool (Element *)> f) override;
  virtual void changed(Core &core, Element *obj) override;

  // Writeable
  virtual DictG getGeneric()  override;

  // Commandable
  virtual void initHUD(HUD *hud) override;
  virtual void setMode(Core &core, HUD *hud) override;
  virtual void processKey(Core &core, SDL_Keycode code) override;
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
  DictO _send;
  DictO _next;
  std::shared_ptr<Flo> _flo;

  void load(Core &core);
  std::optional<DictO> findScenario(std::optional<DictV> scenarios, const std::string &path);
  
};

#endif // H_project_zmqobj
