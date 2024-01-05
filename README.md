<p align="center"><img src="dge_large_logo.png"></p>

# defGameEngine
defGameEngine is a game engine, that's available on Windows, Linux and probably on MacOS

# Installation

Windows:
- Download [GLFW](https://www.glfw.org/download.html).

Linux:
- `sudo apt-get install xorg libx11-dev libglu1-mesa-dev freeglut3-dev libglu1-mesa libgl1-mesa-glx libgl1-mesa-dev libglfw3 libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libxxf86vm-dev libglew-dev libglfw3-dev`

For all platforms:
- Download [defGameEngine.hpp](https://raw.githubusercontent.com/defini7/defGameEngine/master/defGameEngine.hpp).

# Setup

C++20 and higher is required

1. Windows MSVC:
- Menu bar: `Project` -> `<project name> Properties`.
- In opened window `C++` -> `General`.
- Add path for include folders in `Additional include directories`.
- Then `Linker` -> `General`.
- Add path for lib folder in `Additional Library Directories`.

2. Linux G++:
- `g++ <file>.cpp -o <file> -std=c++20 -Wall -lGL -lGLU -lglut -lGLEW -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -fsanitize=undefined -fno-sanitize-recover`.

# License

BSD 3-Clause License

Copyright (c) 2022, 2023, 2024 Alex
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
