/*
  editable.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 25-Aug-2025
    
  Editable interface class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_editable
#define H_editable

class Renderer;

class Editable {

public:
  virtual ~Editable() {};

  virtual std::wstring getString() = 0;
  virtual void setString(Renderer &renderer, const std::wstring &s) = 0;
    // get and set the value as a string.
    
};

#endif // H_editable
