/*
  builder.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 7-Aug-2025
    
  Builder class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_builder
#define H_builder

#include <rfl/json.hpp>
#include <rfl.hpp>
#include <vector>

class Box;
class Pushable;

class Builder {

public:
  
  static Box *walk(const rfl::Generic &g);
  static Box *walk(const rfl::Generic &g, const std::string &name);

  static Box *loadFile(const std::string &fn);
    // load the JSON file into the renderer.
    
  static Box *loadText(const char *text);
    // load the JSON text into the renderer.

  static std::string getJson(Box *box);
    // get the JSON representing the box.
    
private:
  
  static void walk(const rfl::Object<rfl::Generic> &obj, Pushable *list);
  static void walk(const std::vector<rfl::Generic > &v, Pushable *list);

};

#endif // H_builder
