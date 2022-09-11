# defGameEngine
defGameEngine - это игровой движок, доступный на Windows, Linux

# Установка

- Скачать [SDL 2.0](https://github.com/libsdl-org/SDL/releases/tag/release-2.24.0).
- Скачать [SDL_Image 2.0](https://github.com/libsdl-org/SDL_image/releases).
- Скачать файл [defGameEngine.h](https://raw.githubusercontent.com/defini7/defGameEngine/master/defGameEngine.h).

# Запуск

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

На этом все!
