/*
  texteditor.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 18-Aug-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "texteditor.hpp"

#include "colours.hpp"
#include "renderer.hpp"
#include "editable.hpp"
#include "hud.hpp"
#include "unicode.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

using namespace std;

#define CURSOR_BLINK_INTERVAL_MS    500

TextEditor::~TextEditor() {
  if (_text) {
    TTF_DestroyText(_text);
  }
}

void TextEditor::build(Renderer &renderer) {

  _text = TTF_CreateText(renderer._engine, renderer._font->_font, NULL, 0);
  if (!_text) {
    SDL_Log("couldn't create text");
    return;
  }

  // Show the whitespace when wrapping, so it can be edited
//  TTF_SetTextWrapWhitespaceVisible(_text, true);

  // We support rendering the composition and candidates
//  SDL_SetHint(SDL_HINT_IME_IMPLEMENTED_UI, "composition,candidates");

  // make the text black
  TTF_SetTextColor(_text, 0x00, 0x00, 0x00, 0xFF);
  
  _window = renderer._window;
  _renderer = &renderer;
}

void TextEditor::registerHUD(HUD *hud) {

  // the text keys.
  {
    auto mode = new HUDMode(false);
    Renderer::registerGlobalHUDMode(mode);
    mode->add(new Shortcut(L"A", L"ppend"));
    mode->add(new Shortcut(L"S", L"elect all"));
    mode->add(new Shortcut(L"I", L"nsert"));
    mode->add(new Shortcut(L"C", L"opy"));
    mode->add(new Shortcut(L"P", L"aste"));
    mode->add(new Shortcut(L"D", L"elete"));
    _hudtext = hud->registerMode("text", mode);
  }
  
  // keys while editing
  {
    auto mode = new HUDMode(true);
    mode->add(new Shortcut(L"Esc", L"(revert)"));
    mode->add(new Shortcut(L"Arrows", L"(navigate)"));
    mode->add(new Shortcut(L"Bksp", L"(delete)"));
    mode->add(new Shortcut(L"Ret|Tab", L"(close)"));
    _hudediting = hud->registerMode("textedit", mode);
  }

}

void TextEditor::processTextKey(Renderer &renderer, Editable *editable, const Point &origin, const Size &size, SDL_Keycode code, HUD *hud) {

  switch (code) {
    case SDLK_A:
      focus(renderer, origin, size, editable, hud);    
      break;
      
    case SDLK_S:
      focus(renderer, origin, size, editable, hud);
      selectAll();
      break;
      
    case SDLK_I:
      focus(renderer, origin, size, editable, hud);
      mouseDown(renderer, renderer._mouse.x, renderer._mouse.y);
      break;
      
    case SDLK_C:
      {
        auto ws =  editable->getString();
        auto u8str = SDL_iconv_wchar_utf8(ws.c_str());
        SDL_SetClipboardText(u8str);
        TTF_SetTextString(_text, u8str, 0);
        SDL_free(u8str);
      }
      break;

    case SDLK_P:
      {
        auto text = SDL_GetClipboardText();
        editable->setString(renderer, Unicode::convert(text));
      }
      break;
  }

}

bool TextEditor::processEvent(Renderer &renderer, const SDL_Event &event) {

  if (!_editing) {
    return false;
  }
  
  switch (event.type) {
  
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
      mouseDown(renderer, event.button.x, event.button.y);
      break;

    case SDL_EVENT_MOUSE_MOTION:
      mouseMotion(event.motion.x, event.motion.y);
      break;

    case SDL_EVENT_MOUSE_BUTTON_UP:
      mouseUp(event.button.x, event.button.y);
      break;

    case SDL_EVENT_KEY_DOWN:
      switch (event.key.key) {
        case SDLK_A:
            if (event.key.mod& SDL_KMOD_CTRL) {
                selectAll();
            }
            break;
//         case SDLK_C:
//             if (event.key.mod& SDL_KMOD_CTRL) {
//                 copy();
//             }
//             break;
// 
//         case SDLK_V:
//             if (event.key.mod& SDL_KMOD_CTRL) {
//                 paste();
//             }
//             break;
// 
//         case SDLK_X:
//             if (event.key.mod& SDL_KMOD_CTRL) {
//                 cut();
//             }
//             break;

        case SDLK_LEFT:
          if (event.key.mod & SDL_KMOD_CTRL) {
            moveCursorBeginningOfLine();
          } else {
            moveCursorLeft();
          }
          break;

        case SDLK_RIGHT:
          if (event.key.mod & SDL_KMOD_CTRL) {
            moveCursorEndOfLine();
          } else {
            moveCursorRight();
          }
           break;

        case SDLK_UP:
          if (event.key.mod & SDL_KMOD_CTRL) {
            moveCursorBeginning();
          } else {
            moveCursorUp();
          }
          break;

        case SDLK_DOWN:
          if (event.key.mod & SDL_KMOD_CTRL) {
            moveCursorEnd();
          } else {
            moveCursorDown();
          }
          break;

        case SDLK_HOME:
          moveCursorBeginning();
          break;

        case SDLK_END:
          moveCursorEnd();
          break;

        case SDLK_BACKSPACE:
          if (event.key.mod & SDL_KMOD_CTRL) {
            backspaceToBeginning();
          } else {
            backspace();
          }
          break;

        case SDLK_DELETE:
          if (event.key.mod & SDL_KMOD_CTRL) {
            deleteToEnd();
          } else {
            deleteText();
          }
          break;
            
        case SDLK_RETURN:
        case SDLK_TAB:
            endFocus(true);
            break;

        case SDLK_ESCAPE:
            endFocus(false);
            break;

        default:
    //        cout << "got key " << hex << key.key << endl;
          return false;
      }
      break;
      
    case SDL_EVENT_TEXT_INPUT:
      if (!_ignoretext) {
        insert(event.text.text);
      }
      _ignoretext = false;
      break;

    case SDL_EVENT_TEXT_EDITING:
      cout << "text editing" << endl;
      break;

    case SDL_EVENT_TEXT_EDITING_CANDIDATES:
      cout << "text editing candidates" << endl;
      break;

    default:
//        cout << "got event " << hex << event.type << endl;
      return false;
  }
 
  return true;
   
}

void TextEditor::focus(Renderer &renderer, const Point &origin, const Size &size, Editable *obj, HUD *hud) {

  if (!_text) {
    SDL_Log("need to build first!");
    return;
  }
  
  _origin = origin + renderer._offs;
  _size = size;
  _obj = obj;
  
//  TTF_SetTextWrapWidth(_text, _size.w);
  wstring ws =  obj->getString();
  char* u8str = SDL_iconv_wchar_utf8(ws.c_str());
  TTF_SetTextString(_text, u8str, 0);
  SDL_free(u8str);
  
  // start editing at end by default
  SDL_StartTextInput(_window);
  setCursorPosition(ws.size());
  
  _ignoretext = true;
  _editing = true;
  
  // no hilites by default.
  _highlight1 = -1;
  _highlight2 = -1;
  
  // locate the hud on the edited object.
  setHUD(hud);
  hud->setEditingLoc(renderer.localToGlobal(origin));
  
}

void TextEditor::endFocus(bool changed) {

  // unicode?
  if (changed) {
    string s(_text->text);
    _obj->setString(*_renderer, wstring(s.begin(), s.end()));
  }
  _editing = false;
  _renderer->endEdit(_obj);
  
}

void TextEditor::setHUD(HUD *hud) {

  if (_editing) {
    hud->setMode(_hudediting);
  }
  else {
    hud->setMode(_hudtext);
  }
  
}

void TextEditor::selectAll()
{
  if (!_text) {
    SDL_Log("need to build first!");
    return;
  }
  
  _highlight1 = 0;
  _highlight2 = (int)SDL_strlen(_text->text);
}

bool TextEditor::getHighlightExtents(int *marker, int *length)
{
    if (_highlight1 >= 0 && _highlight2 >= 0) {
        int marker1 = SDL_min(_highlight1, _highlight2);
        int marker2 = SDL_max(_highlight1, _highlight2);
        if (marker2 > marker1) {
            *marker = marker1;
            *length = marker2 - marker1;
            return true;
        }
    }
    return false;
}

bool TextEditor::deleteHighlight()
{
  if (!_text) {
    SDL_Log("need to build first!");
    return false;
  }
  
  int marker, length;
  if (getHighlightExtents(&marker, &length)) {
      TTF_DeleteTextString(_text, marker, length);
      setCursorPosition(marker);
      _highlight1 = -1;
      _highlight2 = -1;
      return true;
  }
  return false;
}

void TextEditor::copy()
{
  if (!_text) {
    SDL_Log("need to build first!");
    return;
  }
  
  int marker, length;
  if (getHighlightExtents(&marker, &length)) {
      char *temp = (char *)SDL_malloc(length + 1);
      if (temp) {
          SDL_memcpy(temp, &_text->text[marker], length);
          temp[length] = '\0';
          SDL_SetClipboardText(temp);
          SDL_free(temp);
      }
  } else {
      SDL_SetClipboardText(_text->text);
  }
}

void TextEditor::cut()
{
  if (!_text) {
    SDL_Log("need to build first!");
    return;
  }
  
    /* Copy to clipboard and delete text */
    int marker, length;
    if (getHighlightExtents(&marker, &length)) {
        char *temp = (char *)SDL_malloc(length + 1);
        if (temp) {
            SDL_memcpy(temp, &_text->text[marker], length);
            temp[length] = '\0';
            SDL_SetClipboardText(temp);
            SDL_free(temp);
        }
        TTF_DeleteTextString(_text, marker, length);
        setCursorPosition(marker);
        _highlight1 = -1;
        _highlight2 = -1;
    } else {
        SDL_SetClipboardText(_text->text);
        TTF_DeleteTextString(_text, 0, -1);
    }
}

void TextEditor::paste()
{
  if (!_text) {
    SDL_Log("need to build first!");
    return;
  }
  
  const char *text = SDL_GetClipboardText();
  insert(text);
}

void TextEditor::insert(const char *text) {

  if (!_text) {
    SDL_Log("need to build first!");
    return;
  }
  
  deleteHighlight();

//   if (edit->composition_length > 0) {
//       TTF_DeleteTextString(edit->text, edit->composition_start, edit->composition_length);
//       edit->composition_length = 0;
//   }

  size_t length = SDL_strlen(text);
  TTF_InsertTextString(_text, _cursor, text, length);
  setCursorPosition((int)(_cursor + length));

  // grow the text box
  int w, h;
  TTF_GetTextSize(_text, &w, &h);
  if (w > _size.w) {
    _size.w = w;
  }

}

static int GetCursorTextIndex(int x, const TTF_SubString *substring)
{
    if (substring->flags & (TTF_SUBSTRING_LINE_END | TTF_SUBSTRING_TEXT_END)) {
        return substring->offset;
    }

    bool round_down;
    if ((substring->flags & TTF_SUBSTRING_DIRECTION_MASK) == TTF_DIRECTION_RTL) {
        round_down = (x > (substring->rect.x + substring->rect.w / 2));
    } else {
        round_down = (x < (substring->rect.x + substring->rect.w / 2));
    }
    if (round_down) {
        /* Start the cursor before the selected text */
        return substring->offset;
    } else {
        /* Place the cursor after the selected text */
        return substring->offset + substring->length;
    }
}

void TextEditor::setCursorPosition(int position) {
//     if (edit->composition_length > 0) {
//         /* Don't let the cursor be moved into the composition */
//         if (position >= edit->composition_start && position <= (edit->composition_start + edit->composition_length)) {
//             return;
//         }
// 
//         CancelComposition(edit);
//     }

    _cursor = position;
}

void TextEditor::updateTextInputArea(Renderer &renderer) {

    /* Convert the text input area and cursor into window coordinates */
    SDL_FPoint window_edit_rect_min;
    SDL_FPoint window_edit_rect_max;
    SDL_FPoint window_cursor;
    SDL_FRect r = Rect(_origin, _size).srect();
    if (!SDL_RenderCoordinatesToWindow(renderer._renderer, r.x, r.y, &window_edit_rect_min.x, &window_edit_rect_min.y) ||
        !SDL_RenderCoordinatesToWindow(renderer._renderer, r.x + r.w, r.y + r.h, &window_edit_rect_max.x, &window_edit_rect_max.y) ||
        !SDL_RenderCoordinatesToWindow(renderer._renderer, _cursor_rect.x, _cursor_rect.y, &window_cursor.x, &window_cursor.y)) {
        return;
    }

    SDL_Rect rect;
    rect.x = (int)SDL_roundf(window_edit_rect_min.x);
    rect.y = (int)SDL_roundf(window_edit_rect_min.y);
    rect.w = (int)SDL_roundf(window_edit_rect_max.x - window_edit_rect_min.x);
    rect.h = (int)SDL_roundf(window_edit_rect_max.y - window_edit_rect_min.y);
    int cursor_offset = (int)SDL_roundf(window_cursor.x - window_edit_rect_min.x);
    SDL_SetTextInputArea(renderer._window, &rect, cursor_offset);
}

void TextEditor::drawCursor(Renderer &renderer) {

//     if (edit->composition_length > 0) {
//         DrawCompositionCursor(edit);
//         return;
//     }

    SDL_SetRenderDrawColor(renderer._renderer, 0, 0, 0, 0xFF);
    SDL_RenderFillRect(renderer._renderer, &_cursor_rect);
}

void TextEditor::render(Renderer &renderer, const Point &origin) {

  if (!_editing) {
    return;
  }
  
  /* Clear the text rect to light gray */
  SDL_SetRenderDrawColor(renderer._renderer, 0xCC, 0xCC, 0xCC, 0xFF);
  SDL_FRect r = Rect(_origin, _size).srect();
  SDL_RenderFillRect(renderer._renderer, &r);

  /* Draw any highlight */
  int marker, length;
  if (getHighlightExtents(&marker, &length)) {
      TTF_SubString **highlights = TTF_GetTextSubStringsForRange(_text, marker, length, NULL);
      if (highlights) {
          int i;
          SDL_SetRenderDrawColor(renderer._renderer, 0xEE, 0xEE, 0x00, 0xFF);
          for (i = 0; highlights[i]; ++i) {
              SDL_FRect rect;
              SDL_RectToFRect(&highlights[i]->rect, &rect);
              rect.x += r.x;
              rect.y += r.y;
              SDL_RenderFillRect(renderer._renderer, &rect);
          }
          SDL_free(highlights);
      }
  }
  
  TTF_DrawRendererText(_text, r.x, r.y);
  
    // Draw the cursor
    Uint64 now = SDL_GetTicks();
    if ((now - _last_cursor_change) >= CURSOR_BLINK_INTERVAL_MS) {
        _cursor_visible = !_cursor_visible;
        _last_cursor_change = now;
    }

    /* Calculate the cursor rect, used for positioning candidates */
    TTF_SubString cursor;
    if (TTF_GetTextSubString(_text, _cursor, &cursor)) {

        SDL_RectToFRect(&cursor.rect, &_cursor_rect);
        if ((cursor.flags & TTF_SUBSTRING_DIRECTION_MASK) == TTF_DIRECTION_RTL) {
            _cursor_rect.x += cursor.rect.w;
        }
        _cursor_rect.x += r.x;
        _cursor_rect.y += r.y;
        _cursor_rect.w = 3.0f;

        updateTextInputArea(renderer);
    }

//       if (edit->composition_length > 0) {
//           DrawComposition(edit);
//       }
//   
//       if (edit->candidates) {
//           DrawCandidates(edit);
//       }

    if (_cursor_visible) {
      drawCursor(renderer);
    }
  
}

void TextEditor::moveCursorIndex(int direction)
{
    TTF_SubString substring;

    if (direction < 0) {
        if (TTF_GetTextSubString(_text, _cursor - 1, &substring)) {
            setCursorPosition(substring.offset);
        }
    } else {
        if (TTF_GetTextSubString(_text, _cursor, &substring) &&
            TTF_GetTextSubString(_text, substring.offset + SDL_max(substring.length, 1), &substring)) {
            setCursorPosition(substring.offset);
        }
    }
}

void TextEditor::moveCursorLeft()
{

    TTF_SubString substring;
    if (TTF_GetTextSubString(_text, _cursor, &substring) &&
        (substring.flags & TTF_SUBSTRING_DIRECTION_MASK) == TTF_DIRECTION_RTL) {
        moveCursorIndex(1);
    } else {
        moveCursorIndex(-1);
    }
}

void TextEditor::moveCursorRight()
{

    TTF_SubString substring;
    if (TTF_GetTextSubString(_text, _cursor, &substring) &&
        (substring.flags & TTF_SUBSTRING_DIRECTION_MASK) == TTF_DIRECTION_RTL) {
        moveCursorIndex(-1);
    } else {
        moveCursorIndex(1);
    }
}

void TextEditor::moveCursorUp()
{

//     TTF_SubString substring;
//     if (TTF_GetTextSubString(_text, _cursor, &substring)) {
//         int fontHeight = TTF_GetFontHeight(edit->font);
//         int x, y;
//         if ((substring.flags & TTF_SUBSTRING_DIRECTION_MASK) == TTF_DIRECTION_RTL) {
//             x = substring.rect.x + substring.rect.w - 1;
//         } else {
//             x = substring.rect.x;
//         }
//         y = substring.rect.y - fontHeight / 2;
//         if (TTF_GetTextSubStringForPoint(_text, x, y, &substring)) {
//             SetCursorPosition(edit, GetCursorTextIndex(x, &substring));
//         }
//     }
}

void TextEditor::moveCursorDown()
{

//     TTF_SubString substring;
//     if (TTF_GetTextSubString(_text, _cursor, &substring)) {
//         int fontHeight = TTF_GetFontHeight(edit->font);
//         int x, y;
//         if ((substring.flags & TTF_SUBSTRING_DIRECTION_MASK) == TTF_DIRECTION_RTL) {
//             x = substring.rect.x + substring.rect.w - 1;
//         } else {
//             x = substring.rect.x;
//         }
//         y = substring.rect.y + substring.rect.h + fontHeight / 2;
//         if (TTF_GetTextSubStringForPoint(_text, x, y, &substring)) {
//             SetCursorPosition(edit, GetCursorTextIndex(x, &substring));
//         }
//     }
}

void TextEditor::moveCursorBeginningOfLine()
{

    TTF_SubString substring;
    if (TTF_GetTextSubString(_text, _cursor, &substring) &&
        TTF_GetTextSubStringForLine(_text, substring.line_index, &substring)) {
        setCursorPosition(substring.offset);
    }
}

void TextEditor::moveCursorEndOfLine()
{

    TTF_SubString substring;
    if (TTF_GetTextSubString(_text, _cursor, &substring) &&
        TTF_GetTextSubStringForLine(_text, substring.line_index, &substring)) {
        setCursorPosition(substring.offset + substring.length);
    }
}

void TextEditor::moveCursorBeginning()
{

    /* Move to the beginning of the text */
    setCursorPosition(0);
}

void TextEditor::moveCursorEnd()
{

    /* Move to the end of the text */
    if (_text->text) {
        setCursorPosition((int)SDL_strlen(_text->text));
    }
}

void TextEditor::backspace()
{

    if (deleteHighlight()) {
        return;
    }

    if (_cursor > 0) {
        const char *start = &_text->text[_cursor];
        const char *next = start;
        SDL_StepBackUTF8(_text->text, &next);
        int length = (int)(uintptr_t)(start - next);
        TTF_DeleteTextString(_text, _cursor - length, length);
        _cursor -= length;
    }
}

void TextEditor::backspaceToBeginning()
{

    /* Delete to the beginning of the string */
    TTF_DeleteTextString(_text, 0, _cursor);
    setCursorPosition(0);
}

void TextEditor::deleteToEnd()
{

    /* Delete to the end of the string */
    TTF_DeleteTextString(_text, _cursor, -1);
}

void TextEditor::deleteText()
{
    if (deleteHighlight()) {
        return;
    }

    const char *start = &_text->text[_cursor];
    const char *next = start;
    size_t length = SDL_strlen(next);
    SDL_StepUTF8(&next, &length);
    length = (next - start);
    TTF_DeleteTextString(_text, _cursor, (int)length);
}

void TextEditor::mouseDown(Renderer &renderer, float x, float y)
{
  SDL_FRect r = Rect(_origin, _size).srect();
  
  float scale = 1 / renderer._scale;
  
  SDL_FPoint pt = { x * scale, y * scale };
  if (!SDL_PointInRectFloat(&pt, &r)) {
    return;
  }

  /* Set the cursor position */
  TTF_SubString substring;
  int textX = (int)SDL_roundf((x * scale) - (r.x));
  int textY = (int)SDL_roundf((y * scale) - (r.y));
//  cout << textX << ", " << textY << endl;
  if (!TTF_GetTextSubStringForPoint(_text, textX, textY, &substring)) {
      SDL_Log("Couldn't get cursor location: %s", SDL_GetError());
      return;
  }

  setCursorPosition(GetCursorTextIndex(textX, &substring));
//  edit->highlighting = true;
  _highlight1 = _cursor;
  _highlight2 = -1;
  
}

void TextEditor::mouseMotion(float x, float y)
{
//     if (!edit->highlighting) {
//         return false;
//     }
// 
//     /* Set the highlight position */
//     TTF_SubString substring;
//     int textX = (int)SDL_roundf(x - edit->rect.x);
//     int textY = (int)SDL_roundf(y - edit->rect.y);
//     if (!TTF_GetTextSubStringForPoint(edit->text, textX, textY, &substring)) {
//         SDL_Log("Couldn't get cursor location: %s", SDL_GetError());
//         return false;
//     }
// 
//     SetCursorPosition(edit, GetCursorTextIndex(textX, &substring));
//     edit->highlight2 = edit->cursor;
// 
//     return true;
}

void TextEditor::mouseUp(float x, float y)
{
//     (void)x; (void)y;
// 
//     if (!edit->highlighting) {
//         return false;
//     }
// 
//     edit->highlighting = false;
//     return true;
}
