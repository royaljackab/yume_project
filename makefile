# ==========================================
# 1. DÉTECTION DE L'OS
# ==========================================
ifeq ($(OS),Windows_NT)
    PLATFORM := Windows
    OS_DIR := windows
    TARGET_EXT := .exe
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        PLATFORM := Linux
        OS_DIR := linux
        TARGET_EXT :=
    endif
    ifeq ($(UNAME_S),Darwin)
        PLATFORM := MacOS
        OS_DIR := mac
        TARGET_EXT :=
    endif
endif

# ==========================================
# 2. CONFIGURATION DES CHEMINS
# ==========================================
CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11 -O2
TARGET = danmaku$(TARGET_EXT)

# Dossiers principaux
SRC_DIR = src
LIB_DIR = lib
BIN_DIR = bin

# Chemins des bibliothèques
RAYLIB_PATH = $(LIB_DIR)/raylib/$(OS_DIR)
KOISHI_PATH = $(LIB_DIR)/koishi/$(OS_DIR)

# Inclusion des headers (Couvre l'arborescence symétrique lib/)
INCLUDES = -I$(SRC_DIR) \
           -I$(LIB_DIR) \
           -I$(LIB_DIR)/components \
           -I$(LIB_DIR)/content \
           -I$(LIB_DIR)/content/ui \
           -I$(LIB_DIR)/content/stage \
           -I$(LIB_DIR)/content/stage/moonlight \
           -I$(LIB_DIR)/core \
           -I$(LIB_DIR)/core/coroutine \
           -I$(LIB_DIR)/ecs \
           -I$(LIB_DIR)/systems \
           -I$(LIB_DIR)/raylib/include \
           -I$(LIB_DIR)/koishi/include

# ==========================================
# 3. BIBLIOTHÈQUES PAR PLATEFORME
# ==========================================
# On indique au compilateur où chercher les fichiers .a
LDFLAGS = -L$(RAYLIB_PATH) -L$(KOISHI_PATH) -L$(LIB_DIR)/koishi

ifeq ($(PLATFORM),Windows)
    LIBS = -lraylib -lkoishi -lopengl32 -lgdi32 -lwinmm
    CLEAN_CMD = rmdir /S /Q $(BIN_DIR) & del /Q $(TARGET)
else ifeq ($(PLATFORM),Linux)
    LIBS = -lraylib -lkoishi -lGL -lm -lpthread -ldl -lrt -lX11
    CLEAN_CMD = rm -rf $(BIN_DIR) $(TARGET)
else ifeq ($(PLATFORM),MacOS)
    LIBS = -lraylib -lkoishi -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo
    CLEAN_CMD = rm -rf $(BIN_DIR) $(TARGET)
endif

# ==========================================
# 4. RECHERCHE DES SOURCES ET GESTION DES OBJETS
# ==========================================
# Scan récursif du dossier src/
SRCS = $(wildcard $(SRC_DIR)/*.c) \
       $(wildcard $(SRC_DIR)/*/*.c) \
       $(wildcard $(SRC_DIR)/*/*/*.c) \
       $(wildcard $(SRC_DIR)/*/*/*/*.c) \
       $(wildcard $(SRC_DIR)/*/*/*/*/*.c)

# Transforme les chemins "src/xyz.c" en "bin/xyz.o"
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRCS))

# ==========================================
# 5. RÈGLES DE COMPILATION
# ==========================================
all: $(TARGET)
	@echo "Compilation terminee pour $(PLATFORM)"

# Édition des liens
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS) $(LIBS)

# Compilation des fichiers objets (Création automatique des sous-dossiers dans bin/)
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(CLEAN_CMD)
	@echo "Nettoyage termine (Dossier bin/ et executable supprimes)."

.PHONY: all clean