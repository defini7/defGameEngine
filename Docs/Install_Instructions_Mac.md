# Installation Instructions on MacOS
### Dependencies
1. Download a C++ IDE, but in these instructions we will be using [Xcode](https://developer.apple.com/xcode).

2. Download [GLFW macOS pre-compiled binaries](https://www.glfw.org/download.html).

3. Download the following header files and put into a folder. They will be referenced later as defGameEngine header folder.
    - [defGameEngine.hpp](https://raw.githubusercontent.com/defini7/defGameEngine/master/defGameEngine.hpp)
    - [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h)
    - [stb_image_write.h](https://github.com/nothings/stb/blob/master/stb_image_write.h)

### Setup Under Xcode
1. In Xcode, create a new project and click on `macOS` then `Command Line Tool`. Then complete prompts after until you see your new project window. Now in your new project window, go to the project configuration and click on the project name under `Targets`.

2. Under `General` then `Frameworks and Libraries` add the GLFW library files you have installed:

    - `libglfw.3.dylib`
    - `libglfw3.a`

    Then some frameworks:
    - `AppKit.framework`
    - `CoreFoundation.framework`
    - `CoreGraphics.framework`
    - `CoreHID.framework`
    - `IOKit.framework`
    - `OpenGL.framework`

    Set all additions as `Do Not Embed` except for `libglfw.3.dylib` which will be set as `Embed and Sign`

3. Under `Build Settings` complete the following:
    - Search for `Header Search Paths` then add paths of:     
        - `include` header folder inside GLFW
        -  defGameEngine header folder
    - Search for `Library Search Paths` then add paths of:     
        - `lib` library folder inside GLFW. Use the appropriate library folder coresponding with your system's architecture.
    - Search for `Documentation Comments` and set as `No`
    
4. Now you can sit back, relax, and enjoy coding. You can copy one of the sample games under the `Examples` folder of the repository into `main.cpp` of your project for a quick look and demo. 

**Note:* Warnings are due to the deprecated features of dependencies. They can be safely ignored unless build fails.