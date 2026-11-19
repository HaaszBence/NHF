CC          ?= gcc
CFLAGS      = -std=c11 -Wall -Wextra -O2 -Isrc -Ivendor/raylib

RAYLIB_DIR  = vendor/raylib
RAYLIB_SRCS = $(RAYLIB_DIR)/rcore.c $(RAYLIB_DIR)/rshapes.c $(RAYLIB_DIR)/rtextures.c \
              $(RAYLIB_DIR)/rtext.c $(RAYLIB_DIR)/rmodels.c $(RAYLIB_DIR)/raudio.c \
              $(RAYLIB_DIR)/rglfw.c

rwildcard   = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
GAME_SRCS   = $(call rwildcard,src,*.c)
RESET_BUILD = rm -rf build && mkdir -p build

ifeq ($(OS),Windows_NT)
    TARGET  = build/run.exe
    RUN_CMD = build\run.exe
    DEFS    = -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33
    LDFLAGS = -lopengl32 -lgdi32 -lwinmm
else
    TARGET  = build/run
    RUN_CMD = ./build/run
    DEFS    = -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -D_DEFAULT_SOURCE
    LDFLAGS = -lGL -lm -lpthread -ldl -lrt -lX11
endif

all:
	$(RESET_BUILD)
	$(CC) $(CFLAGS) $(DEFS) -I$(RAYLIB_DIR)/external/glfw/include -w $(RAYLIB_SRCS) $(GAME_SRCS) -o $(TARGET) $(LDFLAGS)

run: all
	$(RUN_CMD)