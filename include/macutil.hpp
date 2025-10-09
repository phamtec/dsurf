/*
  macutil.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 9-Oct-2025
    
  Various Mac specific things.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_macutil
#define H_macutil

#ifdef MAC_APP

#include <string>

class MacUtil {

public:
  
  static std::string getFontPath();
    
};

#endif // MAC_APP

#endif // H_macutil
