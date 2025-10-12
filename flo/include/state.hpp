/*
  state.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 12-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_state
#define H_state

#include <rfl.hpp>

using namespace std;

class State
{
public:
  State();
  State(const State &state);
  
  void setElem(rfl::Object<rfl::Generic> &elem);
  bool hasElem() const;
  const rfl::Object<rfl::Generic> getElem() const;
  
private:
  
  optional<rfl::Object<rfl::Generic>> _elem;
  
};

#endif // H_state
