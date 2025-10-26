/*
  writeable.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 21-Aug-2025
    
  Writeable interface class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_writeable
#define H_writeable

#include <rfl.hpp>

class Element;
class Renderer;

class Writeable {

public:
  virtual ~Writeable() {};

  virtual std::string getName() { return "????"; }
  virtual std::optional<std::string> getPropName() { return std::nullopt; }
  virtual rfl::Generic getGeneric() = 0;
    // return the name and the object for serialization.
    // if there is a name, then the object is a property.
        
  virtual void setDirty(Renderer &renderer, bool state) {}
    // this is dirty all ready  to write!
    
  virtual std::optional<std::string> getFilename() { return std::nullopt; }
    // if the object is file based, return the filename.
    
  static Writeable *cast(Element *obj);
  static Writeable *_err;
  
};

#endif // H_writeable
