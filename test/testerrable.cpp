/*
  testerrable.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 15-Sep-2025
    
  Builder tests.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "element.hpp"
#include "text.hpp"
#include "corner.hpp"
#include "renderer.hpp"
#include "editable.hpp"
#include "hudable.hpp"
#include "keyable.hpp"
#include "parentable.hpp"
#include "pushable.hpp"
#include "sizeable.hpp"
#include "writeable.hpp"

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <fstream>

using namespace std;

// dummy these up so we don't recompile all code
Text::~Text() {
}
void Text::build(Renderer &renderer) {
}
void Text::render(Renderer &renderer, const Point &origin, bool offs) {
}
void Corner::render(Renderer &renderer, const Point &origin) {
}
void Renderer::renderFilledRect(const Rect &rect, const SDL_Color &color) {
}
void Renderer::setScale(double x, double y) {
}
bool Renderer::processRootKey(Element *element, SDL_Keycode code) {
  return false;
}
void Renderer::setTextState() {
}
void Renderer::processTextKey(Editable *editable, const Point &origin, const Size &size, SDL_Keycode code) {
}
SDL_Texture *Renderer::createTexture(int width, int height) {
  return 0;
}
void Renderer::renderTexture(SDL_Texture *texture, const Rect &rect, bool offs) {
}
void Renderer::destroyTexture(SDL_Texture *texture) {
}
void Renderer::setTarget(SDL_Texture *texture) {
}
void Renderer::setDrawColor(const SDL_Color &color) {
}
void Renderer::fillRect(const Rect &r) {
}
void Renderer::clearScale() {
}
void Renderer::restoreScale() {
}
void Renderer::copy(Element *element) {
}
int HUD::registerMode(const std::string &name, HUDMode *mode) {
  return -1;
}
int HUD::findMode(const std::string &name) {
  return -1;
}
void HUDMode::add(Shortcut *shortcut) {
}
Shortcut::Shortcut(const std::wstring &key, const std::wstring &text) {
}
Font::~Font() {
}
Corner::~Corner() {
}

BOOST_AUTO_TEST_CASE( editable )
{
  cout << "=== editable ===" << endl;

  Editable::cast(0)->getString();
  
}

BOOST_AUTO_TEST_CASE( hudable )
{
  cout << "=== hudable ===" << endl;

  HUDable::cast(0)->initHUD(0);
  
}

// BOOST_AUTO_TEST_CASE( keyable )
// {
//   cout << "=== keyable ===" << endl;
// 
//   Renderer r(Size(), 0, 0, Size(), false);
//   Keyable::cast(0)->processKey(r, 0);
//   
// }

BOOST_AUTO_TEST_CASE( parentable )
{
  cout << "=== parentable ===" << endl;

  Parentable::cast(0)->getParent();
  
}

BOOST_AUTO_TEST_CASE( pushable )
{
  cout << "=== pushable ===" << endl;

  Pushable::cast(0)->push(0);
  
}

BOOST_AUTO_TEST_CASE( sizeable )
{
  cout << "=== sizeable ===" << endl;

  Sizeable::cast(0)->getSize();
  
}

BOOST_AUTO_TEST_CASE( writeable )
{
  cout << "=== writeable ===" << endl;

  Writeable::cast(0)->getName();
  
}

