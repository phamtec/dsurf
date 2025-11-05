/*
  colours.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 9-Aug-2025
    
  colours class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_colours
#define H_colours

#include <SDL3/SDL_pixels.h>

class Colours {

public:

  static SDL_Color white;
  static SDL_Color black;
  static SDL_Color red;
  static SDL_Color blue;
  static SDL_Color green;
  static SDL_Color plum;
  static SDL_Color thistle;
  static SDL_Color lavender;
  static SDL_Color orange;
  static SDL_Color grey;
  static SDL_Color lightGrey;
  static SDL_Color lime;
  static SDL_Color turquoise;
  static SDL_Color racingGreen;
  static SDL_Color straw;
  static SDL_Color lightPlum;
  static SDL_Color teaGreen;
  
  // specific colours for elements
  static SDL_Color dictE;
  static SDL_Color listE;
  static SDL_Color stringE;
  static SDL_Color longE;
  static SDL_Color boolE;
  static SDL_Color propertyE;
  static SDL_Color nullE;
  
};

#endif // H_colours
