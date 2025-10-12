/*
  dict.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 12-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_dict
#define H_dict

#include "../function.hpp"

class Dict: public Function
{
public:
  
  // Function
  virtual optional<rfl::Generic> exec(Transform &transform, State *state, rfl::Generic &closure);

  static fPtr create();
  
};

#endif // H_dict
