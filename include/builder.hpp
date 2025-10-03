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

class Builder {

public:
  
  static Element *walk(Element *parent, const rfl::Generic &g);
  static Element *walk(Element *parent, const rfl::Generic &g, const std::string &name);

  static Element *loadFile(const std::string &fn, bool raw=false);
    // load the JSON file into the renderer.
    // if raw is true, then bypass modules.
    
  static Element *loadText(const char *text);
    // load the JSON text into the renderer.

  static std::string getJson(Element *element);
    // get the JSON representing the box.
    
  static void write(Element *element, const std::string &file);
    // write out the generic object.
    
  static std::optional<rfl::Object<rfl::Generic> > getObject(const rfl::Generic &obj);
    // given a generic object, get an Object out of it.
    
  static std::optional<std::string> getString(const rfl::Generic &obj);
    // given a generic object, get a string out of it.
    
  static std::optional<std::vector<rfl::Generic> > getVector(const rfl::Generic &obj);
    // given a generic object, get a vector out of it.

  static std::optional<rfl::Object<rfl::Generic> > getObject(std::optional<rfl::Object<rfl::Generic> > dict, const std::string &name);
    // get an object out of the dictionary with the property name.
    // This is chainable.
    
  static std::optional<std::string> getString(std::optional<rfl::Object<rfl::Generic> > dict, const std::string &name);
    // get a string out of the dictionary with the property name.
    // This is chainable.
    
  static std::optional<std::vector<rfl::Generic> > getVector(std::optional<rfl::Object<rfl::Generic> > dict, const std::string &name);
    // get a string out of the dictionary with the property name.
    // This is chainable.
    


private:
  
  static void walk(Element *parent, const rfl::Object<rfl::Generic> &obj, Element *list);
  static void walk(Element *parent, const std::vector<rfl::Generic> &v, Element *list);
  static Element *castGeneric(const rfl::Generic &g);

};

#endif // H_builder
