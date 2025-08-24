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
  TTF_SetTextWrapWhitespaceVisible(_text, true);

  // We support rendering the composition and candidates
//  SDL_SetHint(SDL_HINT_IME_IMPLEMENTED_UI, "composition,candidates");

  // make the text black
  TTF_SetTextColor(_text, 0x00, 0x00, 0x00, 0xFF);
  
  _window = renderer._window;
}

void TextEditor::processEvent(const SDL_Event &event) {

  switch (event.type) {
  
    case SDL_EVENT_KEY_DOWN:
      switch (event.key.key) {
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
            
        default:
    //        cout << "got key " << hex << key.key << endl;
          break;
      }
      break;
      
    case SDL_EVENT_TEXT_INPUT:
      insert(event.text.text);
      break;

    case SDL_EVENT_TEXT_EDITING:
      cout << "text editing" << endl;
      break;

    case SDL_EVENT_TEXT_EDITING_CANDIDATES:
      cout << "text editing candidates" << endl;
      break;

    default:
//        cout << "got event " << hex << event.type << endl;
      break;
  }
  
}

void TextEditor::focus(const Point &origin, const Size &size, const string &s) {

  if (!_text) {
    SDL_Log("need to build first!");
    return;
  }
  
  _origin = origin;
  _size = size;

  TTF_SetTextWrapWidth(_text, _size.w);
  TTF_SetTextString(_text, s.c_str(), s.size());
  SDL_StartTextInput(_window);
  setCursorPosition(s.size());
  
}

void TextEditor::insert(const char *text) {

  if (!_text) {
    SDL_Log("need to build first!");
    return;
  }
  
//  EditBox_DeleteHighlight(edit);

//   if (edit->composition_length > 0) {
//       TTF_DeleteTextString(edit->text, edit->composition_start, edit->composition_length);
//       edit->composition_length = 0;
//   }

  size_t length = SDL_strlen(text);
  TTF_InsertTextString(_text, _cursor, text, length);
  setCursorPosition((int)(_cursor + length));

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
    SDL_FRect r = Rect(_origin + renderer._offs, _size).srect();
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

  /* Clear the text rect to light gray */
  SDL_SetRenderDrawColor(renderer._renderer, 0xCC, 0xCC, 0xCC, 0xFF);
  SDL_FRect r = Rect(_origin + renderer._offs, _size).srect();
  SDL_RenderFillRect(renderer._renderer, &r);

//   if (_focus) {
//     SDL_FRect focusRect = r;
//     focusRect.x -= 1;
//     focusRect.y -= 1;
//     focusRect.w += 2;
//     focusRect.h += 2;
//     SDL_SetRenderDrawColor(renderer._renderer, 0x00, 0x00, 0x00, 0xFF);
//     SDL_RenderRect(renderer._renderer, &focusRect);
//   }
  
  /* Draw any highlight */
//   int marker, length;
//   if (GetHighlightExtents(edit, &marker, &length)) {
//       TTF_SubString **highlights = TTF_GetTextSubStringsForRange(edit->text, marker, length, NULL);
//       if (highlights) {
//           int i;
//           SDL_SetRenderDrawColor(renderer, 0xEE, 0xEE, 0x00, 0xFF);
//           for (i = 0; highlights[i]; ++i) {
//               SDL_FRect rect;
//               SDL_RectToFRect(&highlights[i]->rect, &rect);
//               rect.x += x;
//               rect.y += y;
//               SDL_RenderFillRect(renderer, &rect);
//           }
//           SDL_free(highlights);
//       }
//   }
  
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

//     if (EditBox_DeleteHighlight(edit)) {
//         return;
//     }
// 
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
//     if (EditBox_DeleteHighlight(edit)) {
//         return;
//     }
// 
    const char *start = &_text->text[_cursor];
    const char *next = start;
    size_t length = SDL_strlen(next);
    SDL_StepUTF8(&next, &length);
    length = (next - start);
    TTF_DeleteTextString(_text, _cursor, (int)length);
}

