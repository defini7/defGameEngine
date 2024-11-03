CC=g++

CC_LIBS=-lGL -lGLU -lglut -lGLEW -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -lstb
CC_FLAGS=-O2 -Wall -pedantic -std=c++20 -fsanitize=undefined -fno-sanitize-recover

define compile
	$(CC) $(CC_FLAGS) -o ../../Target/$(1) ../../../Examples/$(2).cpp $(CC_LIBS)
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
	$(call compile,PlatformGame,PlatformGame)
