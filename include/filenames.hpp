/*
  filenames.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 1-Nov-2025
    
  Utilities to handle file names.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_filenames
#define H_filenames

#include <string>
#include <optional>

class Filenames {

public:

  static std::optional<std::string> isInvalid(const std::string &filename);
    // if the filename is invalid, return why.
    
  static std::string addPath(const std::string &filename);
    // if the file does not have a path, add one in/
    
};

#endif // H_filenames
