# defGameEngine
defGameEngine - это игровой движок, доступный на Windows, Linux

# SDL2

# Установка

- Скачать [SDL 2.0](https://github.com/libsdl-org/SDL/releases/tag/release-2.24.0).
- Скачать [SDL_Image 2.0](https://github.com/libsdl-org/SDL_image/releases).
- Скачать файл [defGameEngine.h](https://raw.githubusercontent.com/defini7/defGameEngine/master/defGameEngine.h).

# Запуск

Используется C++20

1. Windows MinGW:
- Указать путь на папку *lib* и папку *include* SDL2
- Создать файл Makefile и поместить туда следующий текст:
```
OBJS = main.cpp
CC = g++
CC_VERSION = c++17
LINKER_FLAGS = -L
LINKER_TARGET = полный путь к lib
INCLUDE_FLAGS = -I
INCLUDE_TARGET = полный путь к include
OBJ_NAME = out

all: $(OBJS)
	$(CC) -std=$(CC_VERSION) main.cpp $(INCLUDE_FLAGS) $(INCLUDE_TARGET) $(LINKER_FLAGS) $(LINKER_TARGET) -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -o main
```
- Запустить команду `mingw32-make`
- Положить все .dll файлы рядом с исполняемым (.exe) файлом.

2. Windows MSVC:
- На верхней панели нажать ``Проект`` -> ``Свойства: название проекта``.
- В появившемся окне выбрать вкладку ``C++`` -> ``Общие``.
- В поле ``Дополнительные каталоги включаемых библиотек`` вы должны вписать полный путь до папки ```include``` SDL2.
- Далее выбрать вкладку ``Компоновщик`` -> ``Общие``.
- В поле ``Дополнительные каталоги библиотек`` вы должны вписать полный путь до папки ``x86`` или до ``x64``, эти папки находятся в папке ``lib`` SDL2.
- Далее в той же самой вкладке выбрать пункт ``Ввод``.
- В поле ``Дополнительные зависимости`` вписать ```SDL2.lib; SDL2_image.lib; SDL2main.lib;``` к уже существующему там тексту.

3. Linux G++:
- Открыть терминал
- В командной строке написать: 
1) ```sudo apt-get install g++```,
2) ```sudo apt-get install libsdl2-dev```,
3) ```sudo apt-get install libsdl2-image-dev```,
4) ```g++ -o main main.cpp -lSDL2main -lSDL2 -lSDL2_image```.

# GLFW

# Установка

- Скачать [SDL 2.0](https://github.com/libsdl-org/SDL/releases/tag/release-2.24.0).
- Скачать [SDL_Image 2.0](https://github.com/libsdl-org/SDL_image/releases).
- Скачать файл [defGameEngine.h](https://raw.githubusercontent.com/defini7/defGameEngine/master/defGameEngine.h).

# Запуск

Используется C++20

1. Windows MSVC:
- На верхней панели нажать ``Проект`` -> ``Свойства: название проекта``.
- В появившемся окне выбрать вкладку ``C++`` -> ``Общие``.
- В поле ``Дополнительные каталоги включаемых библиотек`` вы должны вписать полный путь до папки ```include``` SDL2.
- Далее выбрать вкладку ``Компоновщик`` -> ``Общие``.
- В поле ``Дополнительные каталоги библиотек`` вы должны вписать полный путь до папки ``x86`` или до ``x64``, эти папки находятся в папке ``lib`` SDL2.
- Далее в той же самой вкладке выбрать пункт ``Ввод``.
- В поле ``Дополнительные зависимости`` вписать ```SDL2.lib; SDL2_image.lib; SDL2main.lib;``` к уже существующему там тексту.

2. Linux G++:
- Открыть терминал
- В командной строке написать: 
1) ```sudo apt-get install xorg libx11-dev libglu1-mesa-dev freeglut3-dev libglu1-mesa libgl1-mesa-glx libgl1-mesa-dev libglfw3 libxrandr-dev 
	libxinerama-dev libxcursor-dev libxi-dev libxxf86vm-dev libglew-dev libglfw3-dev```,
2) ```g++ <source filename>.cpp -o <output filename> -std=c++20 -Wall -lGL -lGLU -lglut -lGLEW -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl
	-lXinerama -lXcursor```,
3) ```g++ <файл>.cpp -o <файл> -std=c++20 -Wall -lGL -lGLU -lglut -lGLEW -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor```,

На этом все!

# Лицензия

BSD 3-Clause License

Copyright (c) 2022, Alex
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
