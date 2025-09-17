/*
  move.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 17-Sep-2025
    
  Utilities for moving objects.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_move
#define H_move

#include <vector>

class Indexable;

class Move {

public:
  
  static void moveObj(std::vector<Indexable *> &objs, int from, int to);

};

#endif // H_move
