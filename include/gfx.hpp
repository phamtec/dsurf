/*
  gfx.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 16-Aug-2025
    
  Graphics class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_gfx
#define H_gfx

class SDL_Renderer;

#include <SDL3/SDL.h>

class Gfx {

public:
  
	static bool pixelRGBA(SDL_Renderer * renderer, 
	  Sint16 x, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	
	static bool vlineRGBA(SDL_Renderer * renderer, 
	  Sint16 x, Sint16 y1, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	  
	static bool hlineRGBA(SDL_Renderer * renderer, 
	  Sint16 x1, Sint16 x2, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	static bool lineRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1,
		Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	static bool boxRGBA(SDL_Renderer * renderer, 
	  Sint16 x1, Sint16 y1, Sint16 x2,
		Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

  static bool roundedBoxRGBA(SDL_Renderer * renderer, 
    Sint16 x1, Sint16 y1, Sint16 x2,
		Sint16 y2, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
		
	static bool arcRGBA(SDL_Renderer * renderer, 
	  Sint16 x, Sint16 y, Sint16 rad, Sint16 start, Sint16 end,
		Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	static bool filledPieRGBA(SDL_Renderer * renderer, 
	  Sint16 x, Sint16 y, Sint16 rad,
		Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
		
	static bool polygonRGBA(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy,
		int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	static bool filledPolygonRGBA(SDL_Renderer * renderer, const Sint16 * vx,
		const Sint16 * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

private:

  static bool _pieRGBA(SDL_Renderer * renderer, 
    Sint16 x, Sint16 y, Sint16 rad, Sint16 start, Sint16 end,  
    Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint8 filled);

};


#endif // H_gfx
