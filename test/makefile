CC = gcc
CFLAGS = -std=gnu11 -g

# On remonte d'un cran pour accéder aux dossiers src et lib
INCLUDES = -I../src \
           -I../lib/internal \
           -I../src/core/coroutine \
           -I../lib/internal/core/coroutine \
           -I../lib/internal/components \
           -I../lib/internal/content \
           -I../lib/internal/content/stage \
           -I../lib/internal/content/stage/moonlight \
           -I../lib/internal/content/stage/moonlight/nonspells \
           -I../lib/internal/content/stage/moonlight/spellcards \
           -I../lib/internal/content/ui \
           -I../lib/internal/core \
           -I../lib/internal/ecs \
           -I../lib/internal/systems \
           -I../lib/raylib/include \
           -I../lib/koishi/include

# Détection OS pour les libs (nécessaire si tes composants utilisent Vector2 de Raylib)
ifeq ($(OS),Windows_NT)
    LDFLAGS = -L../lib/raylib/windows -L../lib/koishi/windows
    LIBS = -lcunit -lraylib -lkoishi -lopengl32 -lgdi32 -lwinmm
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        LDFLAGS = -L../lib/raylib/linux -L../lib/koishi/linux
        LIBS = -lcunit -lraylib -lkoishi -lGL -lm -lpthread -ldl -lrt -lX11
    endif
endif

SRC_DIR = ../src

# 1. On récupère TOUS les fichiers .c
ALL_SRCS = $(wildcard $(SRC_DIR)/*.c) \
           $(wildcard $(SRC_DIR)/*/*.c) \
           $(wildcard $(SRC_DIR)/*/*/*.c) \
           $(wildcard $(SRC_DIR)/*/*/*/*.c) \
           $(wildcard $(SRC_DIR)/*/*/*/*/*.c)

# 2. On EXCLUT le main.c du jeu pour éviter le conflit avec test_main.c
SRC_TO_TEST = $(filter-out $(SRC_DIR)/main.c, $(ALL_SRCS))

TEST_SRCS = test_main.c test_physics.c test_life.c
TARGET = test_suite

all: $(TARGET)
	./$(TARGET)

$(TARGET): $(TEST_SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) $^ $(SRC_TO_TEST) -o $@ $(LDFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean