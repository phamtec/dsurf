/*
  transform.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 12-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_transform
#define H_transform

#include <rfl.hpp>

using namespace std;

class State;
class Functions;

class Transform
{
public:
  Transform(Functions &functions);
  
  optional<rfl::Generic> exec(rfl::Generic &closure, State *state);

  rfl::Generic error(const string &msg) const;

private:
  Functions &_functions;
  
};

#endif // H_transform
