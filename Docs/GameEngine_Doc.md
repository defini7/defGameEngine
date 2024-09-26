# GameEngine Documentation

## Introduction

You can copy one of the sample games under the `Examples` folder of the repository into `.cpp` file of your project for a quick look and demo.

## Table of Contents

1. [Construct Method](#construct-method)
2. [DrawString Function](#drawstring-function)

## Construct Method

Initialize the GameEngine with screen and window settings. Configuration with numerical values for the screen size and pixel width, and boolean values for fullscreen, vsync, and dirty pixel.

#### Parameters:

- `screenWidth`: Width of the screen.
- `screenHeight`: Height of the screen.
- `pixelWidth`: Pixel width on the screen.
- `pixelHeight`: Pixel height on the screen.
- `fullScreen`: Full-screen mode.
- `vsync`: Vertical synchronization.
- `dirtyPixel`: Dirty pixel rendering.

#### Example:
```cpp
GameEngine::Construct(800, 600, 1, 1, false, false, false);
```
This code initializes the GameEngine with a screen size of 800x600 and x and y pixel width by 1. The rest of the parameters are disabled.

## DrawString Function

Draw a string at a specified position with custom color and scaling.

#### Parameters:

- `x`: X-position to start.
- `y`: Y-position to start.
- `s`: String to be drawn.
- `col`: Text color.
- `scaleX`: Scale for x. 
- `scaleY`: Scale for y. 

#### Example:
```cpp
GameEngine::DrawString(10, 10, "Hello World", WHITE, 2, 2);
```
Starting at position (10, 10), the code draws the text "Hello World" in a white color and scales both x and y by 2.
