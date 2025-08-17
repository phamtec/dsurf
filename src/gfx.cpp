/*
  gfx.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 16-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
  
  This code was mainly copy and pasted from:
  
  https://github.com/sabdul-khabir/SDL3_gfx
  https://github.com/rtrussell/BBCSDL
  
  Thank you so much to aschiffler at ferzkopp dot net and 
*/

#include "gfx.hpp"

#include <math.h>

#ifndef M_PI
#define M_PI	3.1415926535897932384626433832795
#endif

/*!
\brief Draw pixel  in currently set color.

\param renderer The renderer to draw on.
\param x X (horizontal) coordinate of the pixel.
\param y Y (vertical) coordinate of the pixel.

\returns Returns true on success, false on failure.
*/
bool pixel(SDL_Renderer *renderer, Sint16 x, Sint16 y)
{
	return SDL_RenderPoint(renderer, x, y);
}

/*!
\brief Draw horizontal line in currently set color

\param renderer The renderer to draw on.
\param x1 X coordinate of the first point (i.e. left) of the line.
\param x2 X coordinate of the second point (i.e. right) of the line.
\param y Y coordinate of the points of the line.

\returns Returns true on success, false on failure.
*/
bool hline(SDL_Renderer * renderer, Sint16 x1, Sint16 x2, Sint16 y)
{
	return SDL_RenderLine(renderer, x1, y, x2, y);;
}

#define AAlevels 256
#define AAbits 8

/*!
\brief Internal function to draw anti-aliased line with alpha blending and endpoint control.

This implementation of the Wu antialiasing code is based on Mike Abrash's
DDJ article which was reprinted as Chapter 42 of his Graphics Programming
Black Book, but has been optimized to work with SDL and utilizes 32-bit
fixed-point arithmetic by A. Schiffler. The endpoint control allows the
supression to draw the last pixel useful for rendering continous aa-lines
with alpha<255.

\param renderer The renderer to draw on.
\param x1 X coordinate of the first point of the aa-line.
\param y1 Y coordinate of the first point of the aa-line.
\param x2 X coordinate of the second point of the aa-line.
\param y2 Y coordinate of the second point of the aa-line.
\param r The red value of the aa-line to draw. 
\param g The green value of the aa-line to draw. 
\param b The blue value of the aa-line to draw. 
\param a The alpha value of the aa-line to draw.
\param draw_endpoint Flag indicating if the endpoint should be drawn; draw if non-zero.

\returns Returns true on success, false on failure.
*/
int Gfx::_aalineRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int draw_endpoint)
{
	Sint32 xx0, yy0, xx1, yy1;
	bool result;
	Uint32 intshift, erracc, erradj;
	Uint32 erracctmp, wgt, wgtcompmask;
	int dx, dy, tmp, xdir, y0p1, x0pxdir;

	/*
	* Keep on working with 32bit numbers 
	*/
	xx0 = x1;
	yy0 = y1;
	xx1 = x2;
	yy1 = y2;

	/*
	* Reorder points to make dy positive 
	*/
	if (yy0 > yy1) {
		tmp = yy0;
		yy0 = yy1;
		yy1 = tmp;
		tmp = xx0;
		xx0 = xx1;
		xx1 = tmp;
	}

	/*
	* Calculate distance 
	*/
	dx = xx1 - xx0;
	dy = yy1 - yy0;

	/*
	* Adjust for negative dx and set xdir 
	*/
	if (dx >= 0) {
		xdir = 1;
	} else {
		xdir = -1;
		dx = (-dx);
	}
	
	/*
	* Check for special cases 
	*/
	if (dx == 0) {
		/*
		* Vertical line 
		*/
		if (draw_endpoint)
		{
			return (vlineRGBA(renderer, x1, y1, y2, r, g, b, a));
		} else {
			if (dy > 0) {
				return (vlineRGBA(renderer, x1, yy0, yy0+dy, r, g, b, a));
			} else {
				return (pixelRGBA(renderer, x1, y1, r, g, b, a));
			}
		}
	} else if (dy == 0) {
		/*
		* Horizontal line 
		*/
		if (draw_endpoint)
		{
			return (hlineRGBA(renderer, x1, x2, y1, r, g, b, a));
		} else {
			if (dx > 0) {
				return (hlineRGBA(renderer, xx0, xx0+(xdir*dx), y1, r, g, b, a));
			} else {
				return (pixelRGBA(renderer, x1, y1, r, g, b, a));
			}
		}
	} else if ((dx == dy) && (draw_endpoint)) {
		/*
		* Diagonal line (with endpoint)
		*/
		return (lineRGBA(renderer, x1, y1, x2, y2,  r, g, b, a));
	}


	/*
	* Line is not horizontal, vertical or diagonal (with endpoint)
	*/
	result = true;

	/*
	* Zero accumulator 
	*/
	erracc = 0;

	/*
	* # of bits by which to shift erracc to get intensity level 
	*/
	intshift = 32 - AAbits;

	/*
	* Mask used to flip all bits in an intensity weighting 
	*/
	wgtcompmask = AAlevels - 1;

	/*
	* Draw the initial pixel in the foreground color 
	*/
	result &= pixelRGBA(renderer, x1, y1, r, g, b, a);

	/*
	* x-major or y-major? 
	*/
	if (dy > dx) {

		/*
		* y-major.  Calculate 16-bit fixed point fractional part of a pixel that
		* X advances every time Y advances 1 pixel, truncating the result so that
		* we won't overrun the endpoint along the X axis 
		*/
		/*
		* Not-so-portable version: erradj = ((Uint64)dx << 32) / (Uint64)dy; 
		*/
		erradj = ((dx << 16) / dy) << 16;

		/*
		* draw all pixels other than the first and last 
		*/
		x0pxdir = xx0 + xdir;
		while (--dy) {
			erracctmp = erracc;
			erracc += erradj;
			if (erracc <= erracctmp) {
				/*
				* rollover in error accumulator, x coord advances 
				*/
				xx0 = x0pxdir;
				x0pxdir += xdir;
			}
			yy0++;		/* y-major so always advance Y */

			/*
			* the AAbits most significant bits of erracc give us the intensity
			* weighting for this pixel, and the complement of the weighting for
			* the paired pixel. 
			*/
			wgt = (erracc >> intshift) & 255;
			result &= pixelRGBAWeight (renderer, xx0, yy0, r, g, b, a, 255 - wgt);
			result &= pixelRGBAWeight (renderer, x0pxdir, yy0, r, g, b, a, wgt);
		}

	} else {

		/*
		* x-major line.  Calculate 16-bit fixed-point fractional part of a pixel
		* that Y advances each time X advances 1 pixel, truncating the result so
		* that we won't overrun the endpoint along the X axis. 
		*/
		/*
		* Not-so-portable version: erradj = ((Uint64)dy << 32) / (Uint64)dx; 
		*/
		erradj = ((dy << 16) / dx) << 16;

		/*
		* draw all pixels other than the first and last 
		*/
		y0p1 = yy0 + 1;
		while (--dx) {

			erracctmp = erracc;
			erracc += erradj;
			if (erracc <= erracctmp) {
				/*
				* Accumulator turned over, advance y 
				*/
				yy0 = y0p1;
				y0p1++;
			}
			xx0 += xdir;	/* x-major so always advance X */
			/*
			* the AAbits most significant bits of erracc give us the intensity
			* weighting for this pixel, and the complement of the weighting for
			* the paired pixel. 
			*/
			wgt = (erracc >> intshift) & 255;
			result &= pixelRGBAWeight (renderer, xx0, yy0, r, g, b, a, 255 - wgt);
			result &= pixelRGBAWeight (renderer, xx0, y0p1, r, g, b, a, wgt);
		}
	}

	/*
	* Do we have to draw the endpoint 
	*/
	if (draw_endpoint) {
		/*
		* Draw final pixel, always exactly intersected by the line and doesn't
		* need to be weighted. 
		*/
		result &= pixelRGBA (renderer, x2, y2, r, g, b, a);
	}

	return (result);
}

/*!
\brief Draw pixel with blending enabled if a<255.

\param renderer The renderer to draw on.
\param x X (horizontal) coordinate of the pixel.
\param y Y (vertical) coordinate of the pixel.
\param r The red color value of the pixel to draw. 
\param g The green color value of the pixel to draw.
\param b The blue color value of the pixel to draw.
\param a The alpha value of the pixel to draw.

\returns Returns true on success, false on failure.
*/
bool Gfx::pixelRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	bool result = true;
	result &= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
	result &= SDL_SetRenderDrawColor(renderer, r, g, b, a);
	result &= SDL_RenderPoint(renderer, x, y);
	return result;
}

/*!
\brief Draw vertical line with blending.

\param renderer The renderer to draw on.
\param x X coordinate of the points of the line.
\param y1 Y coordinate of the first point (i.e. top) of the line.
\param y2 Y coordinate of the second point (i.e. bottom) of the line.
\param r The red value of the line to draw. 
\param g The green value of the line to draw. 
\param b The blue value of the line to draw. 
\param a The alpha value of the line to draw. 

\returns Returns true on success, false on failure.
*/
bool Gfx::vlineRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y1, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	bool result = true;
	result &= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
	result &= SDL_SetRenderDrawColor(renderer, r, g, b, a);
	result &= SDL_RenderLine(renderer, x, y1, x, y2);
	return result;
}

/*!
\brief Draw horizontal line with blending.

\param renderer The renderer to draw on.
\param x1 X coordinate of the first point (i.e. left) of the line.
\param x2 X coordinate of the second point (i.e. right) of the line.
\param y Y coordinate of the points of the line.
\param r The red value of the line to draw. 
\param g The green value of the line to draw. 
\param b The blue value of the line to draw. 
\param a The alpha value of the line to draw. 

\returns Returns true on success, false on failure.
*/
bool Gfx::hlineRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 x2, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	bool result = true;
	result &= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
	result &= SDL_SetRenderDrawColor(renderer, r, g, b, a);
	result &= SDL_RenderLine(renderer, x1, y, x2, y);
	return result;
}

/*!
\brief Draw line with alpha blending.

\param renderer The renderer to draw on.
\param x1 X coordinate of the first point of the line.
\param y1 Y coordinate of the first point of the line.
\param x2 X coordinate of the second point of the line.
\param y2 Y coordinate of the second point of the line.
\param r The red value of the line to draw. 
\param g The green value of the line to draw. 
\param b The blue value of the line to draw. 
\param a The alpha value of the line to draw.

\returns Returns true on success, false on failure.
*/
bool Gfx::lineRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	/*
	* Draw
	*/
	bool result = true;
	result &= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
	result &= SDL_SetRenderDrawColor(renderer, r, g, b, a);	
	result &= SDL_RenderLine(renderer, x1, y1, x2, y2);
	return result;
}

/*!
\brief Draw pixel with blending enabled and using alpha weight on color.

\param renderer The renderer to draw on.
\param x The horizontal coordinate of the pixel.
\param y The vertical position of the pixel.
\param r The red color value of the pixel to draw. 
\param g The green color value of the pixel to draw.
\param b The blue color value of the pixel to draw.
\param a The alpha value of the pixel to draw.
\param weight The weight multiplied into the alpha value of the pixel.

\returns Returns true on success, false on failure.
*/
bool Gfx::pixelRGBAWeight(SDL_Renderer * renderer, Sint16 x, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint32 weight)
{
	/*
	* Modify Alpha by weight 
	*/
	Uint32 ax = a;
	ax = ((ax * weight) >> 8);
	if (ax > 255) {
		a = 255;
	} else {
		a = (Uint8)(ax & 0x000000ff);
	}

	return pixelRGBA(renderer, x, y, r, g, b, a);
}

/*!
\brief Draw anti-aliased polygon with alpha blending.

\param renderer The renderer to draw on.
\param vx Vertex array containing X coordinates of the points of the aa-polygon.
\param vy Vertex array containing Y coordinates of the points of the aa-polygon.
\param n Number of points in the vertex array. Minimum number is 3.
\param r The red value of the aa-polygon to draw. 
\param g The green value of the aa-polygon to draw. 
\param b The blue value of the aa-polygon to draw. 
\param a The alpha value of the aa-polygon to draw.

\returns Returns true on success, false on failure.
*/
bool Gfx::aapolygonRGBA(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	bool result;
	int i;
	const Sint16 *x1, *y1, *x2, *y2;

	/*
	* Vertex array NULL check 
	*/
	if (vx == NULL) {
		return (false);
	}
	if (vy == NULL) {
		return (false);
	}

	/*
	* Sanity check 
	*/
	if (n < 3) {
		return (false);
	}

	/*
	* Pointer setup 
	*/
	x1 = x2 = vx;
	y1 = y2 = vy;
	x2++;
	y2++;

	/*
	* Draw 
	*/
	result = true;
	for (i = 1; i < n; i++) {
		result &= _aalineRGBA(renderer, *x1, *y1, *x2, *y2, r, g, b, a, 0);
		x1 = x2;
		y1 = y2;
		x2++;
		y2++;
	}

	result &= _aalineRGBA(renderer, *x1, *y1, *vx, *vy, r, g, b, a, 0);

	return (result);
}

/*!
\brief Draw anti-aliased filled ellipical pie (or chord) with alpha blending.

\param renderer The renderer to draw on.
\param cx X coordinate of the center of the filled pie.
\param cy Y coordinate of the center of the filled pie.
\param rx Horizontal radius in pixels of the filled pie.
\param ry Vertical radius in pixels of the filled pie.
\param start Starting angle in degrees of the filled pie; zero is right, increasing clockwise.
\param end Ending angle in degrees of the filled pie; zero is right, increasing clockwise.
\param chord Set to 0 for a pie (sector) or 1 for a chord (segment).
\param r The red value of the filled pie to draw. 
\param g The green value of the filled pie to draw. 
\param b The blue value of the filled pie to draw. 
\param a The alpha value of the filled pie to draw.
/
\returns Returns 0 on success, -1 on failure.
*/
int Gfx::aaFilledPieRGBA(SDL_Renderer * renderer, float cx, float cy, float rx, float ry,
	float start, float end, Uint32 chord, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	int nverts, i, result;
	double *vx, *vy;

	// Sanity check radii 
	if ((rx <= 0) || (ry <= 0) || (start == end))
		return -1;

	// Convert degrees to radians
	start = fmod(start, 360.0) * 2.0 * M_PI / 360.0 ;
	end = fmod(end, 360.0) * 2.0 * M_PI / 360.0 ;
	while (start >= end)
		end += 2.0 * M_PI ;

	// Calculate number of vertices on perimeter
	nverts = (end - start) * sqrt(rx * ry) / M_PI ;
	if (nverts < 2)
		nverts = 2 ;
	if (nverts > 180)
		nverts = 180 ;

	// Allocate combined vertex array 
	vx = vy = (double *) malloc(2 * sizeof(double) * (nverts + 1));
	if (vx == NULL)
		return (-1);

	// Update pointer to start of vy
	vy += nverts + 1 ;

	// Calculate vertices:
	for (i = 0; i < nverts; i++)
	    {
		double angle = start + (end - start) * (double) i / (double) (nverts - 1) ; 
		vx[i] = cx + rx * cos(angle);
		vy[i] = cy + ry * sin(angle);
	    }

	// Center:
	vx[i] = cx ;
	vy[i] = cy ;

	result = aaFilledPolygonRGBA(renderer, vx, vy, nverts + 1 - (chord != 0), r, g, b, a);

	// Free combined vertex array
	free(vx);

	return (result);
}

static int _gfxPrimitivesCompareFloat2(const void *a, const void *b)
{
	float diff = *((float *)a + 1) - *((float *)b + 1) ;
	if (diff != 0.0) return (diff > 0) - (diff < 0) ;
	diff = *(float *)a - *(float *)b ;
	return (diff > 0) - (diff < 0) ;
}

// This constant determines the maximum size and/or complexity of polygon that can be
// drawn. Set to 16K the maximum aaArc height is approximately 1100 lines.
#define POLYSIZE 16384

/*!
\brief Draw anti-aliased filled polygon with alpha blending.

\param renderer The renderer to draw on.
\param vx Vertex array containing X coordinates of the points of the filled polygon.
\param vy Vertex array containing Y coordinates of the points of the filled polygon.
\param n Number of points in the vertex array. Minimum number is 3.
\param r The red value of the filled polygon to draw. 
\param g The green value of the filled polygon to draw. 
\param b The blue value of the filed polygon to draw. 
\param a The alpha value of the filled polygon to draw.

\returns Returns 0 on success, -1 on failure, or -2 if the polygon is too large and/or complex.
*/
int Gfx::aaFilledPolygonRGBA(SDL_Renderer * renderer, const double * vx, const double * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	int i, j, xi, yi, result ;
	double x1, x2, y0, y1, y2, minx, maxx, prec ;
	float *list, *strip ;

	if (n < 3)
		return -1 ;

	result = SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) ;

	// Find extrema:
	minx = 99999.0 ;
	maxx = -99999.0 ;
	prec = 0.00001 ;
	for (i = 0; i < n; i++)
	    {
		double x = vx[i] ;
		double y = fabs(vy[i]) ;
		if (x < minx) minx = x ;
		if (x > maxx) maxx = x ;
		if (y > prec) prec = y ;
	    }
	minx = floor (minx) ;
	maxx = floor (maxx) ;
	prec = floor (pow(2,19) / prec) ;

	// Allocate main array, this determines the maximum polygon size and complexity:
	list = (float *) malloc (POLYSIZE * sizeof(float)) ;
	if (list == NULL)
		return -2 ;

	// Build vertex list.  Special x-values used to indicate vertex type:
	// x = -100001.0 indicates /\, x = -100003.0 indicates \/, x = -100002.0 neither
	yi = 0 ;
	y0 = floor(vy[n - 1] * prec) / prec ;
	y1 = floor(vy[0] * prec) / prec ;
	for (i = 1; i <= n; i++)
	    {
		if (yi > POLYSIZE - 4)
		    {
			free (list) ;
			return -2 ;
		    }
		y2 = floor(vy[i % n] * prec) / prec ;
		if (((y1 < y2) - (y1 > y2)) == ((y0 < y1) - (y0 > y1)))
		    {
			list[yi++] = -100002.0 ;
			list[yi++] = y1 ;
			list[yi++] = -100002.0 ;
			list[yi++] = y1 ;
		    }
		else
		    {
			if (y0 != y1)
			    {
				list[yi++] = (y1 < y0) - (y1 > y0) - 100002.0 ;
				list[yi++] = y1 ;
			    }
			if (y1 != y2)
			    {
				list[yi++] = (y1 < y2) - (y1 > y2) - 100002.0 ;
				list[yi++] = y1 ;
			    }
		    }
		y0 = y1 ;
		y1 = y2 ;
	    }
	xi = yi ;

	// Sort vertex list:
	qsort (list, yi / 2, sizeof(float) * 2, _gfxPrimitivesCompareFloat2) ;

	// Append line list to vertex list:
	for (i = 1; i <= n; i++)
	    {
		double x, y ;
		double d = 0.5 / prec ;

		x1 = vx[i - 1] ;
		y1 = floor(vy[i - 1] * prec) / prec ;
		x2 = vx[i % n] ;
		y2 = floor(vy[i % n] * prec) / prec ;

		if (y2 < y1) 
		    {
			double tmp ;
			tmp = x1 ; x1 = x2 ; x2 = tmp ;
			tmp = y1 ; y1 = y2 ; y2 = tmp ;
		    }
		if (y2 != y1)
			y0 = (x2 - x1) / (y2 - y1) ;

		for (j = 1; j < xi; j += 4)
		    {
			y = list[j] ;
			if (((y + d) <= y1) || (y == list[j + 4]))
				continue ;
			if ((y -= d) >= y2)
				break ;
			if (yi > POLYSIZE - 4)
			    {
				free (list) ;
				return -2 ;
			    }
			if (y > y1)
			    {
				list[yi++] = x1 + y0 * (y - y1) ;
				list[yi++] = y ;
			    }
			y += d * 2.0 ;
			if (y < y2)
			    {
				list[yi++] = x1 + y0 * (y - y1) ;
				list[yi++] = y ;
			    }
		    }

		y = floor(y1) + 1.0 ;
		while (y <= y2)
		    {
			x = x1 + y0 * (y - y1) ;
			if (yi > POLYSIZE - 2)
			    {
				free (list) ;
				return -2 ;
			    }
			list[yi++] = x ;
			list[yi++] = y ;
			y += 1.0 ;
		    }
	    }

	// Sort combined list:
	qsort (list, yi / 2, sizeof(float) * 2, _gfxPrimitivesCompareFloat2) ;

	// Plot lines:
	strip = (float *) malloc ((maxx - minx + 2) * sizeof(float)) ;
	if (strip == NULL)
	    {
		free (list) ;
		return -1 ;
	    }
	memset (strip, 0, (maxx - minx + 2) * sizeof(float)) ;
	n = yi ;
	yi = list[1] ;
	j = 0 ;

	for (i = 0; i < n - 7; i += 4)
	    {
		float x1 = list[i + 0] ;
		float y1 = list[i + 1] ;
		float x3 = list[i + 2] ;
		float x2 = list[i + j + 0] ;
		float y2 = list[i + j + 1] ;
		float x4 = list[i + j + 2] ;

		if (x1 + x3 == -200002.0)
			j += 4 ;
		else if (x1 + x3 == -200006.0)
			j -= 4 ;
		else if ((x1 >= minx) && (x2 >= minx))
		    {			
			if (x1 > x2) { float tmp = x1 ; x1 = x2 ; x2 = tmp ; }
			if (x3 > x4) { float tmp = x3 ; x3 = x4 ; x4 = tmp ; }

			for ( xi = x1 - minx; xi <= x4 - minx; xi++ )
			    {
				float u, v ;
				float x = minx + xi ;
				if (x < x2)  u = (x - x1 + 1) / (x2 - x1 + 1) ; else u = 1.0 ;
				if (x >= x3 - 1) v = (x4 - x) / (x4 - x3 + 1) ; else v = 1.0 ;
				if ((u > 0.0) && (v > 0.0))
					strip[xi] += (y2 - y1) * (u + v - 1.0) ;
			    }
		    }

		if ((yi == (list[i + 5] - 1.0)) || (i == n - 8))
		    {
			for (xi = 0; xi <= maxx - minx; xi++)
			    {
				if (strip[xi] != 0.0)
				    {
					if (strip[xi] >= 0.996)
					    {
						int x0 = xi ;
						while (strip[++xi] >= 0.996) ;
						xi-- ;
						result |= SDL_SetRenderDrawColor (renderer, r, g, b, a) ;
						result |= SDL_RenderLine (renderer, minx + x0, yi, minx + xi, yi) ;
					    }
					else
					    {
						result |= SDL_SetRenderDrawColor (renderer, r, g, b, a * strip[xi]) ;
						result |= SDL_RenderPoint (renderer, minx + xi, yi) ;
					    }
				    }
			    }
			memset (strip, 0, (maxx - minx + 2) * sizeof(float)) ;
			yi++ ;

		    }
	    }

	// Free arrays:
	free (list) ;
	free (strip) ;
	return result ;
}
