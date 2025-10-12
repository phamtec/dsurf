/*
  functions.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 12-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_functions
#define H_functions

#include "function.hpp"

#include <map>

using namespace std;

typedef function<shared_ptr<Function> ()> fnHandler;

class Functions
{
public:
    Functions();
    
    bool has(const string &name);
    fPtr get(const string &name);
    
private:
  map<string, fnHandler> _functions;
  
};

#endif // H_functions
