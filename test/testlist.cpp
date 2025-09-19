/*
  testlist.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 19-Sep-2025
    
  Listable tests.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#include "listable.hpp"
#include "element.hpp"
#include "text.hpp"
#include "corner.hpp"
#include "renderer.hpp"

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
Point Renderer::localToGlobal(const Point &p) {
  return p;
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

class Obj: public Element, public Listable {

public:
  Obj(const string &name): _name(name) {}
  
  // Element
  virtual string describe() { return _name; }
  virtual Size layout() { return Size(); }
  virtual void build(Renderer &renderer) {}
  virtual void render(Renderer &renderer, const Point &origin) {}

  // Listable
  virtual void push(Element *element) { _elements.push_back(element); }
  virtual void remove(Renderer &renderer, Element *element) {}
  virtual int count() { return _elements.size(); }
  virtual Element *at(int index) { return _elements[index]; }
  
  string _name;
  vector<Element *> _elements;
  
};

BOOST_AUTO_TEST_CASE( getByPathSimple )
{
  cout << "=== getByPathSimple ===" << endl;
   
  Obj root("root");
  Obj obj1("obj1");
  Obj obj2("obj2");
  Obj obj3("obj3");
  root.push(&obj1);
  root.push(&obj2);
  root.push(&obj3);
  
  auto elem = Listable::getByPath(&root, "/2");
  BOOST_CHECK(elem != nullptr);
  BOOST_CHECK_EQUAL(elem->describe(), "obj3");
  
}

BOOST_AUTO_TEST_CASE( getByPath )
{
  cout << "=== getByPath ===" << endl;
   
  Obj root("root");
  Obj obj1("obj1");
  Obj obj2("obj2");
  Obj obj21("obj21");
  Obj obj22("obj22");
  Obj obj23("obj23");
  Obj obj3("obj3");
  root.push(&obj1);
  root.push(&obj2);
  root.push(&obj3);
  obj2.push(&obj21);
  obj2.push(&obj22);
  obj2.push(&obj23);
  
  auto elem = Listable::getByPath(&root, "/1/2");
  BOOST_CHECK(elem != nullptr);
  BOOST_CHECK_EQUAL(elem->describe(), "obj23");
  
}

BOOST_AUTO_TEST_CASE( getByPathDeeper )
{
  cout << "=== getByPathDeeper ===" << endl;
   
  Obj root("root");
  Obj obj1("obj1");
  Obj obj2("obj2");
  Obj obj21("obj21");
  Obj obj22("obj22");
  Obj obj23("obj23");
  Obj obj231("obj231");
  Obj obj232("obj232");
  Obj obj3("obj3");
  root.push(&obj1);
  root.push(&obj2);
  root.push(&obj3);
  obj2.push(&obj21);
  obj2.push(&obj22);
  obj2.push(&obj23);
  obj23.push(&obj231);
  obj23.push(&obj232);
  
  auto elem = Listable::getByPath(&root, "/1/2/1");
  BOOST_CHECK(elem != nullptr);
  BOOST_CHECK_EQUAL(elem->describe(), "obj232");
  
}


