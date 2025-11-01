/*
  commandable.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 25-Sep-2025
    
  Interface class for dealing with the HUD on objects..
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_commandable
#define H_commandable

#include <SDL3/SDL_events.h>
#include <string>

class HUD;
class Core;
class Element;

class Commandable {

public:
  virtual ~Commandable() {};

  virtual void initHUD(HUD *hud) = 0;
    // for those objects that deal with the HUD direcrly, initialise them.
    
  virtual void setMode(Core &core, HUD *hud) = 0;
    // set the mode in the heads up display.
    
  virtual void processKey(Core &core, SDL_Keycode code) = 0;
    // process the keycode to for object.
    
  virtual std::string getVerb() { return "???"; }
    // return a verb we might use to command this.
    
  static Commandable *cast(Element *obj);
  static Commandable *_err;
  
};

#endif // H_commandable
