# dsurf

Dictionary Surfer

A Dictionary viewer/editor using SDL.

Dictionary types supported:
- JSON
- YAML

You can read a bit about it here:

[Substack article](https://phamtec.substack.com/p/og-gui-programmer-doesnt-know-what)

## Prerequsists

#### Linux

```
sudo apt-get install -y git g++ gcc build-essential git make \
  pkg-config cmake ninja-build gnome-desktop-testing libasound2-dev libpulse-dev \
  libaudio-dev libjack-dev libsndio-dev libx11-dev libxext-dev \
  libxrandr-dev libxcursor-dev libxfixes-dev libxi-dev libxss-dev libxtst-dev \
  libxkbcommon-dev libdrm-dev libgbm-dev libgl1-mesa-dev libgles2-mesa-dev \
  libegl1-mesa-dev libdbus-1-dev libibus-1.0-dev libudev-dev libpipewire-0.3-dev \
  libwayland-dev libdecor-0-dev liburing-dev libfreetype-dev \
  libssl-dev ruby-bundler xclip xsel libyaml-cpp-dev cppzmq-dev
```

#### Mac

```
brew install sdl3 sdl3_ttf bison yaml-cpp pkg-config zmq cppzmq
```

### CMake

We need CMake 3.30.

#### Linux

```
sudo apt remove -y --purge --auto-remove cmake
```

#### From source (Mac and linux)
```
wget https://cmake.org/files/v3.30/cmake-3.30.5.tar.gz
tar -xzvf cmake-3.30.5.tar.gz
cd cmake-3.30.5
./bootstrap
make -j$(nproc)
sudo make install
```

### Boost

#### from source (Mac and linux)

```
wget https://github.com/boostorg/boost/releases/download/boost-1.85.0/boost-1.85.0-b2-nodocs.tar.gz
tar xzf boost-1.85.0-b2-nodocs.tar.gz 
cd boost-1.85.0
./bootstrap.sh --prefix=/usr/local
sudo ./b2 install boost.stacktrace.from_exception=off
```

### SDL

#### from source (Linux)

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

#### from source (Linux)

```
git clone https://github.com/libsdl-org/SDL_ttf
cd SDL_ttf
mkdir build
cd build
cmake ..
make
sudo make install
```

### cpp-reflect

#### from source (Mac and linux)

```
git clone https://github.com/getml/reflect-cpp.git
cd reflect-cpp
git submodule update --init
./vcpkg/bootstrap-vcpkg.sh # Linux, macOS
./vcpkg/bootstrap-vcpkg.bat # Windows
cmake -S . -B build -DCMAKE_CXX_STANDARD=20 -DCMAKE_BUILD_TYPE=Release -DREFLECTCPP_YAML=ON
cmake --build build -j 4 # gcc, clang
cmake --build build --config Release -j 4 # MSVC
cd build
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

## Testing

The end to end tests use cucumber (Ruby), to get that going:

```
sudo bundle install
```

(If your having problems here, like on Intel macs, you might like to install "rvm" which is the ruby version manager to get something like ruby-3.0.3)

You will only need to do that once to install the ruby stuff.

```
./test.sh
```

These aren't ready for prime time on linux but work on a Mac.

## Progress

You can run the app, copy in JSON, edit the JSON nodes as text and then copy it
back out.

## Current Focus

- Editing multiple objects.
  
## Pipeline

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

### 8-Sep-2025

- Version 0.3
- Select All, Copy, Paste and Cut in text editor
- Replace mode which selects all.
- Copy and paste on main mode.
- click to move cursor and Insert mode.

### 12-Sep-2025

- Start work on list editing. Allow the list to be collapsed into a fixed height 
  thing that could be dragged around.

### 17-Sep-2025

- Version 0.4
- Lists can be reordered.

### 18-Sep-2025

- adding and removing.

### 19-Sep-2025

- Cucumber end to end testing.

### 20-Sep-2025

- Show HUD on screen while automated testing.

### 22-Sep-2025

- Undo/Redo for all modifying commands.

### 25-Sep-2025

- Version 0.6
- Edit multiple objects at once.
- Remove Sizeable. All elements are sizeable.
- Remove Parentable. All elements are conceivably parentable.
- Combine HUDable and Keyable into Commandable. Name makes a lot more sense.
- Combine name and elem into a tuple.

### 26-Sep-2025

- Show errors.
- Disable HUD shortcuts for undo/redo

### 29-Sep-2025

- Small scale HUD shows hints instead of null drawing.
- Cast errors show the object description
- start in on being able to paste a dictionary into a property.

### 30-Sep-2025

- Version 0.7
- Pasting a dictionary into another can be undone
- Undo and redo are global now.

### 1-Oct-2025

- Version 0.8
- You can save changes to disk.

### 2-Oct-2025

- Start in on the idea of "modules"
- First module started which is a project.

### 3-Oct-2025

- Projects can load files.
- Add YAML support.

### 6-Oct-2025

- Start in on allowing roots and modules to be rearranged (moved).

### 7-Oct-2025

- Version 0.9
- You can move roots around, and remove them (kill)
- You can add a list of dict as a root.
- You can paste a new dict.
- You can use the 1-9 keys to zoom in and out.

### 10-Oct-2025

- Start work on being able to access remote encrypted servers with ZMQ.

### 11-Oct-2025, 12-Oct-2025, 13-Oct-2025, 14-Oct-2025

- We need F(lo)
- Start in on that.

### 15-Oct-2025

- Built a simple protocol handler that can be coded in F(lo). Documentatioj to come
soon.

### 16-Oct-2025

- Add testability (scenarios) to projects for F(lo) development.

### 18-Oct-2025

- redo the way layout is done to make common code
- merge List and Dict to just have List. They were practically identical.

### 19-Oct-2025

- introduce a discreet code object to simplify drawing and running code.

### 21-Oct-2025

- Version 0.10
- We can run code, change on the fly and see the result.

### 22-Oct-2025

- Use visitor to build and destroy.
- FLo is now Open Source so use that.

### 23-Oct-2025

- Flo adds in YAML.
- New code object that matches the way flo works on the flo readme.

### 24-Oct-2025

- Version 0.11
- layout uses visitor.
- code shows whole scenario on left.
- code shows filename and is writeable.

### 26-Oct-2025

- remove listable. Only used by List so just used that.
- communicate changes to lists for rerunning code.

### 1-Nov-2025

- Allow any dictionary or list to be turned into a transform.
- Allow transforms to output something other than a dictionary.
- Rename "Renderer" class to "Core"
- Big rewrite of the way keys are handled removing large switch statements.

### 2-Nov-2025

- Files can be saved to disk. Not specifying a full path will put it in
  Documents, if there is a path it must exist and the filename must end with
  .json or .yml

### 3-Nov-2025

- Files can be opened from the disk.
- Use SDL file open and save dialogs instead of typing full filename.
- Add support for Null.

### 5-Nov-2025

- Fix list reordering.
- Projects generating and loading others.

### 7-Nov-2025

- Fix empty string crasher.
