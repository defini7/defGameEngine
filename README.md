<p align="center"><img src="dge_large_logo.png"></p>

# defGameEngine
defGameEngine is a game engine, that's available on Windows, Linux and probably on MacOS

# Installation

Windows:
- Download [GLFW](https://www.glfw.org/download.html) (optional).

Linux:
- `sudo apt-get install xorg libx11-dev libglu1-mesa-dev freeglut3-dev libglu1-mesa libgl1-mesa-dev libglfw3 libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libxxf86vm-dev libglew-dev libglfw3-dev`

For all platforms:
- Download [defGameEngine.hpp](https://raw.githubusercontent.com/defini7/defGameEngine/master/defGameEngine.hpp).
- Download [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h).
- Download [stb_image_write.h](https://github.com/nothings/stb/blob/master/stb_image_write.h).

# Setup

C++20 and higher is required

1. Windows MSVC:
   - 1.1 GLFW
      - Menu bar: `Project` -> `<project name> Properties`.
      - In opened window `C++` -> `General`.
      - Add path for include folders in `Additional include directories`.
      - Then `Linker` -> `General`.
      - Add path for lib folder in `Additional Library Directories`.
   - 1.2 OpenGL 1.0 (by default)
      - Sit back, relax and enjoy coding

2. Linux G++:
- `g++ <file>.cpp -o <file> -std=c++20 -Wall -lGL -lGLU -lglut -lGLEW -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -fsanitize=undefined -fno-sanitize-recover`.
