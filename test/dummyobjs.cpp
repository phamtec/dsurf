
#include "element.hpp"
#include "text.hpp"
#include "corner.hpp"
#include "core.hpp"
#include "remotezmq.hpp"

using namespace std;

// dummy these up so we don't recompile all code

// Core
void Core::renderFilledRect(const Rect &rect, const SDL_Color &color) {
}
bool Core::processRootKey(Element *element, SDL_Keycode code) {
  return false;
}
void Core::setTextState() {
}
void Core::processTextKey(Element *element, const Point &origin, const Size &size, SDL_Keycode code) {
}
SDL_Texture *Core::createTexture(int width, int height) {
  return 0;
}
void Core::renderTexture(SDL_Texture *texture, const Rect &rect, bool offs) {
}
void Core::destroyTexture(SDL_Texture *texture) {
}
void Core::setTarget(SDL_Texture *texture) {
}
void Core::setDrawColor(const SDL_Color &color) {
}
void Core::fillRect(const Rect &r) {
}
void Core::clearScale() {
}
void Core::restoreScale() {
}
void Core::copy(Element *element) {
}
Point Core::noOffset(const Point &p) {
  return Point();
}
void Core::initElement(Element *parent, Element *element) {
}
void Core::registerRootHUDMode(HUDMode *mode) {
}
void Core::registerGlobalHUDMode(HUDMode *mode) {
}
void Core::registerTextHUDMode(HUDMode *mode) {
}
void Core::exec(Element *element, Change *change) {
}
void Core::processDeleteKey(Element *element) {
}
void Core::setDirty(Element *elem, bool state) {
}
bool Core::textTooSmall() {
  return false;
}
Element *Core::getClipboard() {
  return nullptr;
}
void Core::setError(const string &str) {
}
void Core::addFile(const string &filename, bool raw) {
}
bool Core::processGlobalKey(SDL_Keycode code) {
  return false;
}
void Core::renderLayout(const Point &origin, const RectList &layout) {
}
void Core::build(Element *elem) {
}
void Core::destroy(Element *elem) {
}
void Core::layout(Element *elem) {
}
void Core::changed(Element *elem) {
}
void Core::connectRemote(const std::string &server, int req, 
    const std::string &upstreamPubKey, const std::string &privateKey, const std::string &pubKey,
    std::shared_ptr<Flo> &flo, const rfl::Object<rfl::Generic> &msg, std::optional<rfl::Object<rfl::Generic> > next) {
}
void Core::addRoot(Element *element, bool useloc) {
}
void Core::removeRoot(Element *element) {
}
bool Core::processKeyHandler(std::map<SDL_Keycode, coreMsgHandler> &handlers, SDL_Keycode code) {
  return false;
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
void HUD::setHint(Core &core, Text *hint) {
}
void HUD::setMode(int mode) {
} 
void HUD::setFlag(Core &core, HUDFlags flag, bool state) {
}
Shortcut::Shortcut(const wstring &key, const wstring &text, optional<HUDFlags> flags) {
}

// Corner
void Corner::render(Core &core, const Point &origin) {
}
