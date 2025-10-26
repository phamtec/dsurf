/*
  fileobj.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 2-Oct-2025
    
  The object element for a Project
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_project_fileobj
#define H_project_fileobj

#include "element.hpp"
#include "text.hpp"
#include "commandable.hpp"
#include "writeable.hpp"

class Element;

class ProjectFileObj: public Element, public Writeable, public Commandable {

public:
  ProjectFileObj(const std::string &name, const std::string &fullfilename, const std::string &filename);

  // Element
  virtual void setParent(Element *parent) override { _parent = parent; }
  virtual Element *getParent() override { return _parent; }
  virtual void build(Renderer &renderer) override;
  virtual void layout() override;
  virtual void render(Renderer &renderer, const Point &origin) override;
  virtual Size size() override { return _size; }

  // Writeable
  virtual rfl::Generic getGeneric() override;

  // Commandable
  virtual void initHUD(HUD *hud) override;
  virtual void setMode(Renderer &renderer, HUD *hud) override;
  virtual void processKey(Renderer &renderer, SDL_Keycode code) override;

protected:

  Element *_parent;
  Size _size;
  Text _name;
  std::string _fullfilename;
  std::string _filename;
  int _hudobj;
  
};

#endif // H_project_fileobj
