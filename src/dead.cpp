/*
  dead.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 25-Jul-2025
    
  Command line driver for dead.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dead
*/

#include <iostream>
#include <boost/program_options.hpp> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;
namespace po = boost::program_options;

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdlib.h>

#define DEFAULT_PTSIZE  18.0f
#define DEFAULT_TEXT    "Hello World"
#define WIDTH   640
#define HEIGHT  480
#define MAX_FALLBACKS 4

typedef struct {
    bool done;
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    SDL_Texture *message;
    SDL_FRect messageRect;
    SDL_FRect textRect;
} Scene;

int main(int argc, char *argv[])
{
    string version = "dead v0.0.1, 25-Jul-2025.";
    
    po::options_description desc("Allowed options");
    desc.add_options()
      ("help", "produce help message")
      ;
    po::positional_options_description p;
  
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
            options(desc).positional(p).run(), vm);
    po::notify(vm);   
  
    if (vm.count("help")) {
      cout << desc << endl;
      return 1;
    }
    
    char *argv0 = argv[0];
    TTF_Font *font = NULL;
    SDL_Surface *text = NULL;
    Scene scene;
    float ptsize;
    int i;
    SDL_Color white = { 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE };
    SDL_Color black = { 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE };
    SDL_Color *forecol;
    SDL_Color *backcol;
    SDL_Event event;
    TTF_TextEngine *engine = NULL;
    int renderstyle = TTF_STYLE_NORMAL;
    int outline = 0;
    int hinting = TTF_HINTING_NORMAL;
    int kerning = 1;
    TTF_HorizontalAlignment align = TTF_HORIZONTAL_ALIGN_LEFT;
    bool editbox = true;
    char *message, string[128];
    int result = 0;

    SDL_zero(scene);

    /* Default is black and white */
    forecol = &black;
    backcol = &white;

    const char *f = "../fonts/911Fonts.com_MonotypeCorsiva_Regular_-_911fonts.com_fonts_gyWJ.ttf";

    /* Initialize the TTF library */
    if (!TTF_Init()) {
        SDL_Log("Couldn't initialize TTF: %s",SDL_GetError());
        result = 2;
        goto done;
    }

    /* Open the font file with the requested point size */
    ptsize = 0.0f;
    i = 2;
    ptsize = DEFAULT_PTSIZE;
    
    font = TTF_OpenFont(f, ptsize);
    if (font == NULL) {
        SDL_Log("Couldn't load %g pt font from %s: %s",
                    ptsize, f, SDL_GetError());
        result = 2;
        goto done;
    }
    
    TTF_SetFontStyle(font, renderstyle);
    TTF_SetFontOutline(font, outline);
    TTF_SetFontKerning(font, kerning);
    TTF_SetFontWrapAlignment(font, align);
    scene.font = font;

    /* Create a window */
    scene.window = SDL_CreateWindow("showfont demo", WIDTH, HEIGHT, 0);
    if (!scene.window) {
        SDL_Log("SDL_CreateWindow() failed: %s", SDL_GetError());
        result = 2;
        goto done;
    }
    scene.renderer = SDL_CreateRenderer(scene.window, NULL);
    if (scene.renderer) {
        SDL_SetRenderVSync(scene.renderer, 1);
    }

    if (!scene.renderer) {
        SDL_Log("SDL_CreateRenderer() failed: %s", SDL_GetError());
        result = 2;
        goto done;
    }

    engine = TTF_CreateRendererTextEngine(scene.renderer);
    if (!engine) {
        SDL_Log("Couldn't create renderer text engine: %s", SDL_GetError());
        result = 2;
        goto done;
    }

    /* Show which font file we're looking at */
    SDL_snprintf(string, sizeof(string), "Font file: %s", f);  /* possible overflow */

    /* Render and center the message */
    message = DEFAULT_TEXT;
    text = TTF_RenderText_Shaded(font, message, 0, *forecol, *backcol);
    if (text == NULL) {
        SDL_Log("Couldn't render text: %s", SDL_GetError());
        result = 2;
        goto done;
    }
    scene.messageRect.x = (float)((WIDTH - text->w)/2);
    scene.messageRect.y = (float)((HEIGHT - text->h)/2);
    scene.messageRect.w = (float)text->w;
    scene.messageRect.h = (float)text->h;
    scene.message = SDL_CreateTextureFromSurface(scene.renderer, text);
    SDL_Log("Font is generally %d big, and string is %d big",
                        TTF_GetFontHeight(font), text->h);

    if (editbox) {
        scene.textRect.x = 8.0f;
        scene.textRect.y = 4.0f;
        scene.textRect.w = WIDTH / 2 - scene.textRect.x * 2;
        scene.textRect.h = scene.messageRect.y - scene.textRect.y - 16.0f;

        SDL_FRect editRect = scene.textRect;
        editRect.x += 4.0f;
        editRect.y += 4.0f;
        editRect.w -= 8.0f;
        editRect.w -= 8.0f;
    }

    /* Wait for a keystroke, and blit text on mouse press */
    while (!scene.done) {
        while (SDL_PollEvent(&event)) {
            SDL_ConvertEventToRenderCoordinates(scene.renderer, &event);

            switch (event.type) {

                case SDL_EVENT_QUIT:
                    scene.done = true;
                    break;

                default:
                    break;
            }
        }
        SDL_SetRenderDrawColor(scene.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(scene.renderer);
    
        SDL_RenderTexture(scene.renderer, scene.message, NULL, &scene.messageRect);
        SDL_RenderPresent(scene.renderer);
    }
    result = 0;

done:
    SDL_DestroySurface(text);
    TTF_CloseFont(font);
    SDL_DestroyTexture(scene.message);
    TTF_Quit();
    SDL_Quit();
    return result;
}
