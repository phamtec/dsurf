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

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <SDL3/SDL_opengl.h>

using namespace std;
namespace po = boost::program_options;

#define DEFAULT_PTSIZE  18.0f
#define DEFAULT_TEXT    "Hello World"
#define WIDTH           640
#define HEIGHT          480

static void SDL_GL_Enter2DMode(int width, int height)
{
    /* Note, there may be other things you need to change,
       depending on how you have your OpenGL state set up.
    */
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);

    /* This allows alpha blending of 2D textures with the scene */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glOrtho(0.0, (GLdouble)width, (GLdouble)height, 0.0, 0.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

static void SDL_GL_Leave2DMode(void)
{
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glPopAttrib();
}

/* Quick utility function for texture creation */
static int power_of_two(int input)
{
    int value = 1;

    while (value < input) {
        value <<= 1;
    }
    return value;
}

static GLuint SDL_GL_LoadTexture(SDL_Surface *surface, GLfloat *texcoord)
{
    GLuint texture;
    int w, h;
    SDL_Surface *image;
    SDL_Rect area;
    Uint8  saved_alpha;
    SDL_BlendMode saved_mode;

    /* Use the surface width and height expanded to powers of 2 */
    w = power_of_two(surface->w);
    h = power_of_two(surface->h);
    texcoord[0] = 0.0f;         /* Min X */
    texcoord[1] = 0.0f;         /* Min Y */
    texcoord[2] = (GLfloat)surface->w / w;  /* Max X */
    texcoord[3] = (GLfloat)surface->h / h;  /* Max Y */

    image = SDL_CreateSurface(w, h, SDL_PIXELFORMAT_RGBA32);
    if (image == NULL) {
        return 0;
    }

    /* Save the alpha blending attributes */
    SDL_GetSurfaceAlphaMod(surface, &saved_alpha);
    SDL_SetSurfaceAlphaMod(surface, 0xFF);
    SDL_GetSurfaceBlendMode(surface, &saved_mode);
    SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);

    /* Copy the surface into the GL texture image */
    area.x = 0;
    area.y = 0;
    area.w = surface->w;
    area.h = surface->h;
    SDL_BlitSurface(surface, &area, image, &area);

    /* Restore the alpha blending attributes */
    SDL_SetSurfaceAlphaMod(surface, saved_alpha);
    SDL_SetSurfaceBlendMode(surface, saved_mode);

    /* Create an OpenGL texture for the image */
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,
             0,
             GL_RGBA,
             w, h,
             0,
             GL_RGBA,
             GL_UNSIGNED_BYTE,
             image->pixels);
    SDL_DestroySurface(image); /* No longer needed */

    return texture;
}

static void cleanup(int exitcode)
{
    TTF_Quit();
    SDL_Quit();
    exit(exitcode);
}

int main(int argc, char *argv[]) {

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
  
  SDL_Window *window;
  SDL_GLContext context;
  TTF_Font *font;
  SDL_Surface *text = NULL;
  int done;
  SDL_Color white = { 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE };
  SDL_Color black = { 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE };
  GLenum gl_error;
  GLuint texture;
  int x, y, w, h;
  GLfloat texcoord[4];
  GLfloat texMinX, texMinY;
  GLfloat texMaxX, texMaxY;
  SDL_Event event;

  /* Initialize the TTF library */
  if (!TTF_Init()) {
      fprintf(stderr, "Couldn't initialize TTF: %s\n",SDL_GetError());
      SDL_Quit();
      return(2);
  }

  /* Open the font file with the requested point size */
  const char *f = "../fonts/911Fonts.com_MonotypeCorsiva_Regular_-_911fonts.com_fonts_gyWJ.ttf";
  font = TTF_OpenFont(f, DEFAULT_PTSIZE);
  if (font == NULL) {
      fprintf(stderr, "Couldn't load %g pt font from %s: %s\n",
                  DEFAULT_PTSIZE, f, SDL_GetError());
      cleanup(2);
  }
  TTF_SetFontStyle(font, TTF_STYLE_NORMAL);

  /* Set a 640x480 video mode */
  window = SDL_CreateWindow("dead", WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
  if (window == NULL) {
      fprintf(stderr, "Couldn't create window: %s\n", SDL_GetError());
      cleanup(2);
  }

  context = SDL_GL_CreateContext(window);
  if (context == NULL) {
      fprintf(stderr, "Couldn't create OpenGL context: %s\n", SDL_GetError());
      cleanup(2);
  }

  /* Render and center the message */
  text = TTF_RenderText_Blended(font, DEFAULT_TEXT, 0, black);
  if (text == NULL) {
      fprintf(stderr, "Couldn't render text: %s\n", SDL_GetError());
      TTF_CloseFont(font);
      cleanup(2);
  }
  x = (WIDTH - text->w)/2;
  y = (HEIGHT - text->h)/2;
  w = text->w;
  h = text->h;
  printf("Font is generally %d big, and string is %d big\n",
    TTF_GetFontHeight(font), text->h);

  /* Convert the text into an OpenGL texture */
  glGetError();
  texture = SDL_GL_LoadTexture(text, texcoord);
  if ((gl_error = glGetError()) != GL_NO_ERROR) {
      /* If this failed, the text may exceed texture size limits */
      printf("Warning: Couldn't create texture: 0x%x\n", gl_error);
  }

  /* Make texture coordinates easy to understand */
  texMinX = texcoord[0];
  texMinY = texcoord[1];
  texMaxX = texcoord[2];
  texMaxY = texcoord[3];

  /* We don't need the original text surface anymore */
  SDL_DestroySurface(text);

  /* Initialize the GL state */
  glViewport(0, 0, WIDTH, HEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(-2.0, 2.0, -2.0, 2.0, -20.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glShadeModel(GL_SMOOTH);

  /* Wait for a keystroke, and blit text on mouse press */
  done = 0;
  while (!done) {
      while (SDL_PollEvent(&event)) {
          switch (event.type) {
              case SDL_EVENT_KEY_DOWN:
              case SDL_EVENT_QUIT:
              done = 1;
              break;
              default:
              break;
          }
      }

      /* Clear the screen */
      glClearColor(1.0, 1.0, 1.0, 1.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      /* Show the text on the screen */
      SDL_GL_Enter2DMode(WIDTH, HEIGHT);
      glBindTexture(GL_TEXTURE_2D, texture);
      glBegin(GL_TRIANGLE_STRIP);
      glTexCoord2f(texMinX, texMinY); glVertex2i(x,   y);
      glTexCoord2f(texMaxX, texMinY); glVertex2i(x+w, y);
      glTexCoord2f(texMinX, texMaxY); glVertex2i(x,   y+h);
      glTexCoord2f(texMaxX, texMaxY); glVertex2i(x+w, y+h);
      glEnd();
      SDL_GL_Leave2DMode();

      /* Swap the buffers so everything is visible */
      SDL_GL_SwapWindow(window);
  }
  SDL_GL_DestroyContext(context);
  TTF_CloseFont(font);
  cleanup(0);

  /* Not reached, but fixes compiler warnings */
  return 0;
}
