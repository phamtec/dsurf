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

#include <rfl.hpp>

class Element;

class ProjectZMQObj: public Element, public Commandable {

public:
  ProjectZMQObj(const std::string &name, const rfl::Object<rfl::Generic> &obj);

  // Element
  virtual void setParent(Element *parent) override { _parent = parent; }
  virtual Element *getParent() override { return _parent; }
  virtual void build(Renderer &renderer) override;
  virtual void destroy(Renderer &renderer) override;
  virtual Size layout() override;
  virtual void render(Renderer &renderer, const Point &origin) override;
  virtual Size size() override { return _size; }

  // Commandable
  virtual void initHUD(HUD *hud) override;
  virtual void setMode(Renderer &renderer, HUD *hud) override;
  virtual void processKey(Renderer &renderer, SDL_Keycode code) override;

protected:

  Element *_parent;
  Size _size;
  Text _name;
  int _hudobj;
  bool _editing;
  std::vector<std::unique_ptr<Text> > _subheadings;
  std::vector<std::unique_ptr<Element> > _code;
  std::vector<rfl::Generic > _source;
  
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
  void createObjects(Renderer &renderer);
  
};

#endif // H_project_zmqobj
