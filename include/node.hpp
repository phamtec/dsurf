/*
  node.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 6-Aug-2025
    
  Node class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dead
*/

#ifndef H_node
#define H_node

#include "text.hpp"
#include "list.hpp"

class Node {

public:
  Node(): _obj(std::nullopt) {}
  
  static Node makeList();
  static Node makeObject();
  static Node makeNull();

  void setString(const std::string &s) {
    _obj = Text(s);
  }  

  void setBool(bool b) {
    _obj = Text(b ? "true" : "false");
  }  

private:
  
  std::variant<Text, List, std::nullopt_t> _obj;

};

#endif // H_node
