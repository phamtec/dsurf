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

#include "dict.hpp"

using vops::DictG;
using vops::DictO;
using vops::DictV;

class Element;

class Builder {

public:
  
  static Element *walk(Element *parent, const DictG &g);
  static Element *walk(Element *parent, const DictG &g, const std::string &name);

 static Element *loadObj(const DictG &obj, const std::string &fn);
    // load the Generic object into the core.
    
 static Element *loadFile(const std::string &fn, bool raw=false);
    // load the JSON file into the core.
    // if raw is true, then bypass modules.
    
  static Element *loadText(const char *text);
    // load the JSON text into the core.

  static std::string getJson(Element *element);
    // get the JSON representing the element.
    
  static std::string getYaml(Element *element);
    // get the YAML representing the element.

  static void write(const DictG &g, const std::string &file);
    // write out the generic object.
    
private:
  
  static void walk(Element *parent, const DictO &obj, Element *list);
  static void walk(Element *parent, const DictV &v, Element *list);
  static Element *castGeneric(const DictG &g);

};

#endif // H_builder
