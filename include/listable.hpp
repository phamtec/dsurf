/*
  listable.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 6-Aug-2025
    
  List utilities.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_listable
#define H_listable

#include <string>

class Element;
class List;

class Listable {

public:

  static Element *getByPath(List *root, const std::string &path);
    // given a root, get the element by the path given.

private:

  static Element *getByIndex(List *elem, const std::string &str);

};

#endif // H_listable
