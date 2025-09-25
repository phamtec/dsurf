
#include "element.hpp"
#include "text.hpp"
#include "corner.hpp"
#include "renderer.hpp"

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
bool Renderer::processRootKey(Element *element, SDL_Keycode code) {
  return false;
}
void Renderer::setTextState() {
}
void Renderer::processTextKey(Element *element, const Point &origin, const Size &size, SDL_Keycode code) {
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
Point Renderer::noOffset(const Point &p) {
  return Point();
}
void Renderer::initElement(Element *parent, int index, Element *element) {
}
void Renderer::registerRootHUDMode(HUDMode *mode) {
}
void Renderer::registerTextHUDMode(HUDMode *mode) {
}
void Renderer::exec(Element *element, Change *change) {
}
void Renderer::processDeleteKey(Element *element) {
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
