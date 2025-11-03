/*
  colours.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 9-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "colours.hpp"

SDL_Color Colours::white = { 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE };
SDL_Color Colours::black = { 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE };
SDL_Color Colours::red = { 0xFF, 0x00, 0x00, SDL_ALPHA_OPAQUE };
SDL_Color Colours::blue = { 0x00, 0x00, 0x80, SDL_ALPHA_OPAQUE };
SDL_Color Colours::green = { 0x00, 0x64, 0x00, SDL_ALPHA_OPAQUE };
SDL_Color Colours::plum = { 0x8E, 0x45, 0x85, SDL_ALPHA_OPAQUE };
SDL_Color Colours::orange = { 0xFF, 0xA5, 0x00, SDL_ALPHA_OPAQUE };
SDL_Color Colours::grey = { 0x80, 0x80, 0x80, SDL_ALPHA_OPAQUE };
SDL_Color Colours::lightGrey = { 0xF0, 0xF0, 0xF0, SDL_ALPHA_OPAQUE };
SDL_Color Colours::lime = { 0x8B, 0xB0, 0x6F, SDL_ALPHA_OPAQUE };
SDL_Color Colours::turquoise = { 0x53, 0xB6, 0xAC, SDL_ALPHA_OPAQUE };
SDL_Color Colours::racingGreen = { 0x22, 0x55, 0x22, SDL_ALPHA_OPAQUE };
SDL_Color Colours::straw = { 0xF4, 0xEF, 0xC5, SDL_ALPHA_OPAQUE };
SDL_Color Colours::lightPlum = { 0xD8, 0xBC, 0xCD, SDL_ALPHA_OPAQUE };
SDL_Color Colours::teaGreen = { 0xd0, 0xf0, 0xc0, SDL_ALPHA_OPAQUE };

SDL_Color Colours::dictE = plum;
SDL_Color Colours::listE = orange;
SDL_Color Colours::stringE = green;
SDL_Color Colours::longE = turquoise;
SDL_Color Colours::boolE = lime;
SDL_Color Colours::propertyE = blue;
SDL_Color Colours::nullE = black;
