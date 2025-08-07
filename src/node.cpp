/*
  node.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 6-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dead
*/

#include "node.hpp"

Node Node::makeList() {
  Node n;
  n._obj = List(false);
  return n;
}

Node Node::makeObject() {
  Node n;
  n._obj = List(true);
  return n;
}
