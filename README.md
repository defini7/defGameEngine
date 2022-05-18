# defGameEngine
defGameEngine - это кроссплатформенный игровой движок, доступный на Windows, Linux, MacOS

# Установка

- Скачать [SDL 2.0](https://www.libsdl.org/download-2.0.php). Нам нужно Development Libraries.
- Скачать [SDL_Image 2.0](https://www.libsdl.org/projects/SDL_image/). Нам нужно Development Libraries.
- Скачать файл [defGameEngine.h](https://raw.githubusercontent.com/defini7/defGameEngine/master/defGameEngine.h).

# Запуск с помощью GNU компилятора

1. Windows:
```
OBJS = main.cpp
CC = g++
CC_VERSION = c++17
LINKER_FLAGS = -L
LINKER_TARGET = C:\repos\Test\SDL2\lib
INCLUDE_FLAGS = -I
INCLUDE_TARGET = C:\repos\Test\SDL2\include
OBJ_NAME = out

all: $(OBJS)
	$(CC) -std=$(CC_VERSION) main.cpp $(INCLUDE_FLAGS) $(INCLUDE_TARGET) $(LINKER_FLAGS) $(LINKER_TARGET) -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -o main
```

# Важно!
- Класс Sprite не доработан в версии OpenGL... пока что.
- Версия OpenGL поддерживается только на Windows.

На этом все!
