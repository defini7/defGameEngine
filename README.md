# defGameEngine
defGameEngine - это кроссплатформенный игровой движок, доступный на Windows, Linux, MacOS

# Установка

- Скачать [SDL 2.0](https://www.libsdl.org/download-2.0.php). Нам нужно Development Libraries.
- Скачать [SDL_Image 2.0](https://www.libsdl.org/projects/SDL_image/). Нам нужно Development Libraries.
- Скачать файл [defGameEngine.h](https://raw.githubusercontent.com/defini7/defGameEngine/master/defGameEngine.h).

# Запуск

1. Windows MinGW:
- Указать путь на папку lib и папку include SDL2
- Создать файл Makefile и поместить туда следующий текст:
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
- Запустить команду mingw32-make
- Положить все .dll файлы рядом с исполняемым (.exe) файлом.

2. Windows MSVC:
- На верхней панели нажать ``Проект`` -> ``Свойства: название проекта``.
- В появившемся окне выбрать вкладку ``C++`` -> ``Общие``.
- В поле ``Дополнительные каталоги включаемых библиотек`` вы должны вписать полный путь до папки ```include``` SDL2.
- Далее выбрать вкладку ``Компоновщик`` -> ``Общие``.
- В поле ``Дополнительные каталоги библиотек`` вы должны вписать полный путь до папки ``x86`` или до ``64``, эти папки находятся в папке ``lib`` SDL2.
- Далее в той же самой вкладке выбрать пункт ``Ввод``.
- В поле ``Дополнительные зависимости`` вписать ```SDL2.lib; SDL2_image.lib; SDL2main.lib;``` к уже существующему там тексту.

# Важно!
- Класс Sprite не доработан в версии OpenGL... пока что.
- Версия OpenGL поддерживается только на Windows.

На этом все!
