CC=g++

CC_GLFW_INCLUDE=C:\SDK\glfw\include
CC_GLFW_LIB=C:\SDK\glfw\x64\lib-mingw-w64
CC_STB_INCLUDE=C:\SDK\stb
CC_LIBS=-lgdi32 -luser32 -lkernel32 -lopengl32 -lglfw3dll -lglu32

CC_FLAGS=-Wall -pedantic -std=c++20 -fms-extensions
CC_DEPENDENCIES=-I"$(CC_STB_INCLUDE)" -I"$(CC_GLFW_INCLUDE)" -L"$(CC_GLFW_LIB)" $(CC_LIBS)

define compile
	$(CC) $(CC_FLAGS) -o ../../Target/$(1) ../../../Examples/$(2).cpp $(CC_DEPENDENCIES)
endef

.PHONY: all
all:
	$(call compile,Asteroids,Asteroids)
	$(call compile,AxisKiller,AxisKiller)
	$(call compile,BreakOut,BreakOut)
	$(call compile,CAD,CAD)
	$(call compile,Camera2D,Camera2D)
	$(call compile,Checkers,Checkers)
	$(call compile,ColorPalette,ColorPalette)
	$(call compile,ConsoleExample,ConsoleExample)
	$(call compile,Cube3D,Cube3D)
	$(call compile,DDA_Raycaster2D,DDA_Raycaster2D)
	$(call compile,DDA_Raycaster3D,DDA_Raycaster3D)
	$(call compile,Fireworks,Fireworks)
	$(call compile,Jackpot,Jackpot)
	$(call compile,Matrices,Matrices)
	$(call compile,Mazes,Mazes)
	$(call compile,Mode7,Mode7)
	$(call compile,PathFinder,PathFinder)
	$(call compile,PerlinNoise,PerlinNoise)
	$(call compile,PlatformGame,PlatformGame)
	$(call compile,Plots,Plots)
	$(call compile,Pool,Pool)
	$(call compile,SlidingPuzzle,SlidingPuzzle)
	$(call compile,Snake,Snake)
	$(call compile,Snow,Snow)
	$(call compile,Splines,Splines)
	$(call compile,StarField,StarField)
	$(call compile,Tetris,Tetris)
	$(call compile,Timer,Timer)
	$(call compile,Yahtzee,Yahtzee)
