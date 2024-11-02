# Installation Instructions on Emscripten
### Dependencies

1. Download [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html).

2. Download the following header files.
    - [defGameEngine.hpp](https://raw.githubusercontent.com/defini7/defGameEngine/master/defGameEngine.hpp)
    - [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h)
    - [stb_image_write.h](https://github.com/nothings/stb/blob/master/stb_image_write.h)

### Setup

1. Edit [build file](https://github.com/defini7/defGameEngine/blob/master/Build/Scripts/Emscripten/cpp_to_web.bat) by specifying correct paths for the libraries

2. Build with
```console
cpp_to_web build <file_without_extension>
```

3. Run with
```console
cpp_to_web run
```

### Examples
1. [ConsoleDemo](https://defini7.itch.io/defgameengine-consoledemo)
2. [Raycaster3D](https://defini7.itch.io/defgameengine-raycaster)
