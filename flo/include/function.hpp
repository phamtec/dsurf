/*
  function.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 12-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_function
#define H_function

#include <rfl.hpp>

using namespace std;

class Transform;
class State;

class Function;

typedef shared_ptr<Function> fPtr;

class Function
{
public:

    virtual optional<rfl::Generic> exec(Transform &transform, State *state, rfl::Generic &closure) = 0;

};

#endif // H_function
