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

  super::build(renderer);
  
  _text = TTF_CreateText(renderer._engine, renderer._font->_font, NULL, 0);
  if (!_text) {
    SDL_Log("couldn't create text");
    return;
  }

  // Show the whitespace when wrapping, so it can be edited
  TTF_SetTextWrapWhitespaceVisible(_text, true);

  // We support rendering the composition and candidates
  SDL_SetHint(SDL_HINT_IME_IMPLEMENTED_UI, "composition,candidates");

  _window = renderer._window;
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
  _focus = true;
  _cursor = 0;
  
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

  if (_focus) {
    SDL_FRect focusRect = r;
    focusRect.x -= 1;
    focusRect.y -= 1;
    focusRect.w += 2;
    focusRect.h += 2;
    SDL_SetRenderDrawColor(renderer._renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderRect(renderer._renderer, &focusRect);
  }
  
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
  
  if (_focus) {
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
          _cursor_rect.w = 1.0f;
  
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
  
}

