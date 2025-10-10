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

#include <rfl.hpp>

class Element;

class ProjectZMQObj: public Element, public Commandable {

public:
  ProjectZMQObj(const std::string &name, const rfl::Object<rfl::Generic> &obj);

  // Element
  virtual void setParent(Element *parent) { _parent = parent; }
  virtual Element *getParent() { return _parent; }
  virtual void build(Renderer &renderer);
  virtual Size layout();
  virtual void render(Renderer &renderer, const Point &origin);
  virtual Size size() { return _size; }

  // Commandable
  virtual void initHUD(HUD *hud);
  virtual void setMode(Renderer &renderer, HUD *hud);
  virtual void processKey(Renderer &renderer, SDL_Keycode code);

protected:

  Element *_parent;
  Size _size;
  Text _name;
  int _hudobj;
  
  std::string _remoteAddress;
  int _remotePort;
  std::string _remotePubKey;
  std::string _uuid;
  std::string _privateKey;
  std::string _publicKey;

  void load(Renderer &renderer);
  
};

#endif // H_project_zmqobj
