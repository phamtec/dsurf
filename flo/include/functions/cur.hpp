/*
  cur.hpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 15-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_cur
#define H_cur

#include "../function.hpp"

namespace flo {

class Cur: public Function
{
public:
  
  // Function
  virtual optional<rfl::Generic> exec(Transform &transform, State *state, rfl::Generic &closure);

  static fPtr create();
  
};

} // flo

#endif // H_cur
