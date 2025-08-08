/*
  textpool.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 8-Aug-2025
    
  Text pool class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_textpool
#define H_textpool

#include "text.hpp"

class TextPool {

public:
  TextPool(): open_brace("{"), close_brace("}"), open_bracket("["), close_bracket("]") {}
  
  Text open_brace;
  Text close_brace;
  Text open_bracket;
  Text close_bracket;
  
};

#endif // H_textpool
