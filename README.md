# dsurf

Dictionary Surfer

A Dictionary viewer/editor using SDL.

## Prerequsists

#### Linux

```
sudo apt-get install -y git g++ gcc build-essential git make \
  pkg-config cmake ninja-build gnome-desktop-testing libasound2-dev libpulse-dev \
  libaudio-dev libjack-dev libsndio-dev libx11-dev libxext-dev \
  libxrandr-dev libxcursor-dev libxfixes-dev libxi-dev libxss-dev libxtst-dev \
  libxkbcommon-dev libdrm-dev libgbm-dev libgl1-mesa-dev libgles2-mesa-dev \
  libegl1-mesa-dev libdbus-1-dev libibus-1.0-dev libudev-dev libpipewire-0.3-dev \
  libwayland-dev libdecor-0-dev liburing-dev libfreetype-dev
```

### Boost

#### from source

```
wget https://github.com/boostorg/boost/releases/download/boost-1.85.0/boost-1.85.0-b2-nodocs.tar.gz
tar xzf boost-1.85.0-b2-nodocs.tar.gz 
cd boost-1.85.0
./bootstrap.sh --prefix=/usr
sudo ./b2 install boost.stacktrace.from_exception=off
```

#### Mac

```
brew install boost@1.85.0
```

### SDL

#### Mac

```
brew install sdl3
```

#### from source

```
git clone https://github.com/libsdl-org/SDL
cd SDL
mkdir build
cd build
cmake ..
make
sudo make install
```

### SDL TTF support

#### Mac

```
brew install sdl3_ttf
```

#### from source

```
git clone https://github.com/libsdl-org/SDL_ttf
cd SDL_ttf
mkdir build
cd build
cmake ..
make
sudo make install
```

### JSON

#### from source

```
git clone https://github.com/getml/reflect-cpp.git
cd reflect-cpp
mkdir build
cd build
cmake ..
make
sudo make install
```

## Building this project

```
mkdir build
cd build
cmake ..
make
make test
```

## Running this

```
./dsurf ../test/complex.json
```

Use the mouse wheel (or double finger up and down on a traockpad) to zoom.
Double click to zoom in where you clicked. Shift+Double click to zoom back out.
Click and drag to pan around.

Keys:
- Hit q to exit.
- Follow the heads up display for other keys.
- Arrows left and write to navigate through editing text.
- Backspace to delete text

## Running tests

The tests use BOOST test, to run

```
make test
```

## Interactive debugging on a Mac

There is a cool little script you can use like this in a command window:

```
cd build
../run.sh ../test ./TestSpatial scaleAndCenter
```

This will monitor the "test" directory for changes to a file, if it finds them it will do a "make" and then if that is successful it will run the program "./TestSpatial" specifying the test "scaleAndCenter" to run.

You can split your screen to see this command window and your text editor and interactively make changes, save and see results :-)

If you want to compile things that change in the source dir, just change the command to

```
../run.sh ../src ./TestSpatial scaleAndCenter
```

## Progress

You can run the app, copy in JSON, edit the JSON nodes as text and then copy it
back out.

## Current Focus

- HUD
- Bug fixes
- Distribution on mac and linux
- Windows version

## Attributions

### Graphics primitives

- https://github.com/sabdul-khabir/SDL3_gfx
- Thank you so much to aschiffler at ferzkopp dot net!

## Change Log

### 25-Jul-2025

Hello world. First checkin.

### 8-Aug-2025

Walk a JSON file building a simple layout.

### 9-Aug-2025

- nice colours.
- get screen size for window.
- lot's of better layout work.
- panning and zooming.

### 10-Aug-2025

- Work to center while zooming.

### 11-Aug-2025

- First go at centering while zooming.

### 12-Aug-2025

- Get the math right for an object at an offset already.

### 13-Aug-2025

- Crack the math to keep the view centered right at the cursor while zooming.

### 14-Aug-2025

- Draw orange and plum objects instead of brackets and braces.

### 15-Aug-2025

- Allow JSON on the clipbaord to be pasted into the window.
- Allow JSON to be copied from the window.

### 16-Aug-2025

- Double click and shift to zoom in our out.
- Don't keep surface around with text. just texture.
- Nicer drawing.

### 17-Aug-2025

- prerender corners of the dict and oversample 4 times to make them look 
  nicer zooed in.
  
### 18-Aug-2025

- Start in on hit testing
  
### 20-Aug-2025

- Cleanup rendering.
  
### 21-Aug-2025

- Ctrl-E will edit and then when you click on a stringbox the editor will appear.
- The editor allows you to insert characters at the start but doesn't save etc.
- First go at real editing.

### 22-Aug-2025

- More work on Editing
- simplify code base with a Property class.

### 24-Aug-2025

- Everything is editable now. Still no saving or complex editing.
- arrow keys, backspace and delete all work.

 ### 25-Aug-2025

- Version 0.1
- You can edit any existing text, hit escape and then copy the new dictionary to the clipboard.

 ### 5-Sep-2025

- Version 0.2
- Heads up display for editing.

### 6-Sep-2025

- Introduce Hudable and Keyable and seperate HUD and Key concerns to objects.
- Grow edit box
- Nicer HUD
- Unicode

### 7-Sep-2025

- Dynamic HUD work. No fixed modes.
- text editor owns it's modes
- renderer owns root modes.



