/*
  font.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 4-Aug-2025
    
  Font class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dead
*/

#ifndef H_font
#define H_font

class TTF_Font;

class Font {

public:
  Font(): _font(0) {}
  ~Font();
  
  bool init(const char *path);

private:
  friend class Text;
  
  TTF_Font *_font;
  
};

#endif // H_font
