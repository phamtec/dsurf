/*
  texteditor.hpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 18-Aug-2025
    
  Text editor class.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifndef H_texteditor
#define H_texteditor

#include "element.hpp"

#include <SDL3/SDL_events.h>

class TTF_Text;
class SDL_Window;
class Editable;
class HUD;

class TextEditor: public Element {

  typedef Element super;
  
public:
  TextEditor(bool editing): _text(0), _window(0), _cursor(0), _renderer(0), _obj(0), _editing(editing) {}
  ~TextEditor();
  
  bool processEvent(Renderer &renderer, const SDL_Event &event);
  bool capture() { return _editing; }
  void registerHUD(HUD *hud);
  void setHUD(HUD *hud);
  void processTextKey(Renderer &renderer, Editable *editable, const Point &origin, const Size &size, SDL_Keycode code, HUD *hud);
  
  // Element
  virtual void build(Renderer &renderer);
  virtual Size layout() { return _size; };
  virtual void render(Renderer &renderer, const Point &origin);
  virtual Size size() { return Size(); }
  
  Point _origin;
  Size _size;

private:
  
  TTF_Text *_text;
  SDL_Window *_window;
  bool _cursor_visible;
  Uint64 _last_cursor_change;
  SDL_FRect _cursor_rect;
  int _cursor;
  Renderer *_renderer;
  Editable *_obj;
  bool _ignoretext;
  bool _editing;
  int _hudtext;
  int _hudediting;
  int _highlight1;
  int _highlight2;
  
  void focus(Renderer &renderer, const Point &origin, const Size &size, Editable *obj, HUD *hud);
  void updateTextInputArea(Renderer &renderer);
  void drawCursor(Renderer &renderer);
  void setCursorPosition(int position);
  void insert(const char *text);
  void selectAll();
  void copy();
  void paste();
  void cut();
  void moveCursorIndex(int direction);
  void moveCursorLeft();
  void moveCursorRight();
  void moveCursorUp();
  void moveCursorDown();
  void moveCursorBeginningOfLine();
  void moveCursorEndOfLine();
  void moveCursorBeginning();
  void moveCursorEnd();
  void backspace();
  void backspaceToBeginning();
  void deleteToEnd();
  void deleteText();
  void endFocus(bool changed);
  bool getHighlightExtents(int *marker, int *length);
  bool deleteHighlight();
  void mouseDown(Renderer &renderer, float x, float y);
  void mouseMotion(float x, float y);
  void mouseUp(float x, float y);

};

#endif // H_texteditor
