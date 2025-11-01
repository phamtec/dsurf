/*
  shortcut.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 5-Sep-2025
    
  A shortcut for the heads up display.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_shortcut
#define H_shortcut

#include "text.hpp"
#include "point.hpp"
#include "hudflags.hpp"

#include <optional>
#include <memory>

class Core;

class Shortcut {

public:
  Shortcut(const std::wstring &key, const std::wstring &text, std::optional<HUDFlags> flags = std::nullopt);
  
  void build(Core &core);
  void render(Core &core, const Point &origin);
  
  Size size();
  
  bool setFlag(HUDFlags flag, bool state);

private:

  std::unique_ptr<Text> _key;
  std::unique_ptr<Text> _text;
  std::optional<HUDFlags> _flags;
  bool _state;

  void setup(const std::wstring &key, const std::wstring &text);
  
};

#endif // H_shortcut
