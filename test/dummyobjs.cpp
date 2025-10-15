
#include "element.hpp"
#include "text.hpp"
#include "corner.hpp"
#include "renderer.hpp"

using namespace std;

// dummy these up so we don't recompile all code

// Renderer
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
void Renderer::initElement(Element *parent, Element *element) {
}
void Renderer::registerRootHUDMode(HUDMode *mode) {
}
void Renderer::registerGlobalHUDMode(HUDMode *mode) {
}
void Renderer::registerTextHUDMode(HUDMode *mode) {
}
void Renderer::exec(Element *element, Change *change) {
}
void Renderer::processDeleteKey(Element *element) {
}
void Renderer::setDirty(Element *elem, bool state) {
}
bool Renderer::textTooSmall() {
  return false;
}
Element *Renderer::getClipboard() {
  return nullptr;
}
void Renderer::setError(const string &str) {
}
void Renderer::addFile(const string &filename, bool raw) {
}
bool Renderer::processGlobalKey(SDL_Keycode code) {
  return false;
}
bool Renderer::setupRemote(const string &server, int req, 
  const string &remotePubKey, const string &privateKey, const string &pubKey) {
  return false;
}
void Renderer::startRemote(std::unique_ptr<Flo> &flo, const rfl::Object<rfl::Generic> &msg, const rfl::Object<rfl::Generic> &next) {
}
void Renderer::sendRemote(const rfl::Object<rfl::Generic> &msg) {
}

// HUD
int HUD::registerMode(const string &name, HUDMode *mode) {
  return -1;
}
int HUD::findMode(const string &name) {
  return -1;
}
void HUDMode::add(Shortcut *shortcut) {
}
void HUD::setHint(Renderer &renderer, Text *hint) {
}
void HUD::setMode(int mode) {
} 
Shortcut::Shortcut(const wstring &key, const wstring &text, optional<HUDFlags> flags) {
}

// Other
Text::~Text() {
}
void Text::build(Renderer &renderer) {
}
void Text::render(Renderer &renderer, const Point &origin, bool offs) {
}
void Corner::render(Renderer &renderer, const Point &origin) {
}

