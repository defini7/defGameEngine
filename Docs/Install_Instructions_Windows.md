# Installation Instructions on Windows
### Dependencies

1. Download [GLFW Windows pre-compiled binaries](https://www.glfw.org/download.html) with your corresponding system architecture.

2. Download the following header files and put into a folder.
    - [defGameEngine.hpp](https://raw.githubusercontent.com/defini7/defGameEngine/master/defGameEngine.hpp)
    - [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h)
    - [stb_image_write.h](https://github.com/nothings/stb/blob/master/stb_image_write.h)

### Setup

1. Windows Microsoft Visual C++ (MSVC):

    - GLFW:
        - Menu bar: `Project` -> `<project name> Properties`.
        - In opened window `C++` -> `General`.
        - Add path for include folders in `Additional include directories`.
        - Then `Linker` -> `General`.
        - Add path for lib folder in `Additional Library Directories`.
    
2. MinGW

    - Edit [Makefile](https://github.com/defini7/defGameEngine/blob/master/Build/Scripts/Windows/Makefile) by specifying correct paths for the libraries

    - Compile with:
    ```console
    mingw32-make
    ```

    - Run with:
    ```console
    ./<file>
    ```
