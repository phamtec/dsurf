/*
  keyable.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 6-Sep-2025
    
  Interface interface class for sending keys to an object.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_keyable
#define H_keyable

#include <SDL3/SDL_events.h>

class Renderer;
class Element;

class Keyable {

public:
  virtual ~Keyable() {};

  virtual void processKey(Renderer &renderer, SDL_Keycode code) = 0;
    // process the keycode to for object.
    
  static Keyable *cast(Element *obj);
  static Keyable *_err;
    
};

#endif // H_keyable
