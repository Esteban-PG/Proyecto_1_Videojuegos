CC      = g++
STD     = -std=c++17
CFLAGS  = -Wall -Wextra -g
INC_PATH = -I"./libs/" \
            -I/opt/homebrew/include \
            -I/opt/homebrew/Cellar/lua@5.4/5.4.8/include/lua5.4

SRC = src/*.cpp \
      src/game/*.cpp \
      src/ECS/*.cpp \
      src/asset_manager/*.cpp \
      src/audio_manager/*.cpp \
      src/controller_manager/*.cpp \
      src/scene_manager/*.cpp \
      libs/tinyxml2/tinyxml2.cpp

LFLAGS = -L/opt/homebrew/lib \
          -L/opt/homebrew/Cellar/lua@5.4/5.4.8/lib \
          -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua5.4

build:
	mkdir -p bin
	$(CC) $(STD) $(CFLAGS) $(INC_PATH) $(SRC) -o bin/game_engine $(LFLAGS)

run:
	./bin/game_engine

clean:
	rm -rf bin
