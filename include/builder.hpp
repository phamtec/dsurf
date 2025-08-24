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

class Element;
class Pushable;

class Builder {

public:
  
  static Element *walk(Element *parent, int index, const rfl::Generic &g);
  static Element *walk(Element *parent, int index, const rfl::Generic &g, const std::string &name);

  static Element *loadFile(const std::string &fn);
    // load the JSON file into the renderer.
    
  static Element *loadText(const char *text);
    // load the JSON text into the renderer.

  static std::string getJson(Element *element);
    // get the JSON representing the box.
    
private:
  
  static void walk(Element *parent, const rfl::Object<rfl::Generic> &obj, Pushable *list);
  static void walk(Element *parent, const std::vector<rfl::Generic > &v, Pushable *list);
  static Element *castGeneric(const rfl::Generic &g);

};

#endif // H_builder
