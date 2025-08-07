/*
  list.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 6-Aug-2025
    
  List class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dead
*/

#ifndef H_list
#define H_list

#include <vector>
#include <memory>

class Node;

class List {

public:
  List(bool object): _object(object) {}
  
private:
  
  bool _object;
  std::vector<std::unique_ptr<Node> > _objs;
  
};

#endif // H_list
