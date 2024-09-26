# Installation Instructions on Linux
### Dependencies

1. Run the following code in terminal:
```console
sudo apt-get install xorg libx11-dev libglu1-mesa-dev freeglut3-dev libglu1-mesa libgl1-mesa-dev libglfw3 libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libxxf86vm-dev libglew-dev libglfw3-dev
```

2. Download the following header files and put into a folder.
    - [defGameEngine.hpp](https://raw.githubusercontent.com/defini7/defGameEngine/master/defGameEngine.hpp)
    - [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h)
    - [stb_image_write.h](https://github.com/nothings/stb/blob/master/stb_image_write.h)

### Setup

G++
```console
g++ <file>.cpp -o <file> -std=c++20 -Wall -lGL -lGLU -lglut -lGLEW -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -fsanitize=undefined -fno-sanitize-recover
```