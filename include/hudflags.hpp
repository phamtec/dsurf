/*
  hudflags.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 26-Sep-2025
    
 FLags for the HUD.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_hudflags
#define H_hudflags

typedef enum {
  
  none = 0,
  canUndo = (1 << 1),
  canRedo = (1 << 2)

} HUDFlags;

#endif // H_hudflags
