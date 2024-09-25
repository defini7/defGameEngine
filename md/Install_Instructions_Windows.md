# Installation Instructions on Windows
### Dependencies

1. Download [GLFW Windows pre-compiled binaries](https://www.glfw.org/download.html) with your corresponding system architecture. *optional*

2. Download the following header files and put into a folder.
    - [defGameEngine.hpp](https://raw.githubusercontent.com/defini7/defGameEngine/master/defGameEngine.hpp)
    - [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h)
    - [stb_image_write.h](https://github.com/nothings/stb/blob/master/stb_image_write.h)

### Setup

Windows Microsoft Visual C++ (MSVC):

1. GLFW
    - Menu bar: `Project` -> `<project name> Properties`.
    - In opened window `C++` -> `General`.
    - Add path for include folders in `Additional include directories`.
    - Then `Linker` -> `General`.
    - Add path for lib folder in `Additional Library Directories`.
    
2. OpenGL 1.0 (by default)
    - Sit back, relax and enjoy coding