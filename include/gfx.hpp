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
  
  static int aaFilledPieRGBA(SDL_Renderer * renderer, 
    float cx, float cy, float rx, float ry,
		float start, float end, Uint32 chord, 
		Uint8 r, Uint8 g, Uint8 b, Uint8 a);
			
private:

	static bool pixelRGBA(SDL_Renderer * renderer, 
	  Sint16 x, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	
	static bool vlineRGBA(SDL_Renderer * renderer, 
	  Sint16 x, Sint16 y1, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	  
	static bool hlineRGBA(SDL_Renderer * renderer, 
	  Sint16 x1, Sint16 x2, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	static bool lineRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1,
		Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	static bool aapolygonRGBA(SDL_Renderer * renderer, 
	  const Sint16 * vx, const Sint16 * vy,
		int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

  static int _aalineRGBA(SDL_Renderer * renderer, 
    Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, 
    Uint8 r, Uint8 g, Uint8 b, Uint8 a, int draw_endpoint);
    
  static bool pixelRGBAWeight(SDL_Renderer * renderer, 
    Sint16 x, Sint16 y, 
    Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint32 weight);

	static int aaFilledPolygonRGBA(SDL_Renderer * renderer, 
	  const double * vx, const double * vy, int n, 
	  Uint8 r, Uint8 g, Uint8 b, Uint8 a);

};


#endif // H_gfx
