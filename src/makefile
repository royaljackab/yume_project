# Nom de l'exécutable
TARGET_NAME = jeu
# Dossier de destination (à la racine du projet)
BIN_DIR = ../bin
# Chemin complet de l'exécutable
TARGET = $(BIN_DIR)/$(TARGET_NAME)

CC = gcc

# Détection de l'OS
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    # Linux
    LIBS = -lraylib -lm -lpthread -ldl -lrt -lX11 -lkoishi
    LIBPATH = -L../lib
    NEED_RAYLIB_BUILD = false
    NEED_KOISHI_BUILD = true
else ifeq ($(UNAME_S),Darwin)
    # macOS
    LIBS = -lraylib -lm -lpthread -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -lkoishi
    LIBPATH = -L/opt/homebrew/lib -L../lib
    NEED_RAYLIB_BUILD = false
    NEED_KOISHI_BUILD = false
else
    # Windows (MSYS2, MinGW)
    LIBS = -lraylib -lm -lpthread -lopengl32 -lgdi32 -lwinmm   # plus de -lkoishi
    LIBPATH = -L../lib -L../include/src
    NEED_RAYLIB_BUILD = true
    NEED_KOISHI_BUILD = true
endif

# Dossiers des sources et bibliothèques
RAYLIB_SRC_DIR = ../include/src
RAYLIB_LIB = $(RAYLIB_SRC_DIR)/libraylib.a

KOISHI_SRC_DIR = ../include/koishi
KOISHI_INCLUDE_DIR = $(KOISHI_SRC_DIR)/include
KOISHI_BUILD_DIR = $(KOISHI_SRC_DIR)/build
KOISHI_LIB = ../lib/libkoishi.a

# Règles conditionnelles pour Windows
ifeq ($(NEED_RAYLIB_BUILD),true)
$(RAYLIB_LIB):
	@echo "Raylib library not found, compiling it..."
	cd $(RAYLIB_SRC_DIR) && make PLATFORM=PLATFORM_DESKTOP
endif

ifeq ($(NEED_KOISHI_BUILD),true)
# Vérifier que meson et ninja sont installés
MESON := $(shell command -v meson 2> /dev/null)
NINJA := $(shell command -v ninja 2> /dev/null)
ifndef MESON
$(error "meson is not installed. Please install it (pacman -S meson)")
endif
ifndef NINJA
$(error "ninja is not installed. Please install it (pacman -S ninja)")
endif

$(KOISHI_LIB):
	@echo "Koishi library not found or invalid, compiling it with Meson..."
	# Supprimer l'ancien répertoire de build pour repartir de zéro
	rm -rf $(KOISHI_BUILD_DIR)
	mkdir -p $(KOISHI_BUILD_DIR)
	cd $(KOISHI_BUILD_DIR) && meson setup .. --default-library=static --buildtype=release
	cd $(KOISHI_BUILD_DIR) && ninja
	# Copier la bibliothèque uniquement (le header est déjà en place)
	mkdir -p $(dir $(KOISHI_LIB))
	cp $(KOISHI_BUILD_DIR)/src/libkoishi.a $(KOISHI_LIB)
	@echo "Koishi compilation successful."
endif

# Recherche des dossiers de header dans ../lib
LIB_DIRS = $(shell find ../lib -type d)
# Inclusion des headers
INCLUDES = -I. $(addprefix -I,$(LIB_DIRS)) -I../include -I$(RAYLIB_SRC_DIR) -I$(KOISHI_INCLUDE_DIR)

# Flags de compilation
CFLAGS = -g $(INCLUDES)

# Recherche de tous les fichiers .c dans src/ et ../lib/
SRC = $(shell find . -type f -name "*.c") $(shell find ../lib -type f -name "*.c")
OBJ = $(addprefix $(BIN_DIR)/, $(notdir $(SRC:.c=.o)))

# Règles principales
all: $(BIN_DIR) $(TARGET)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compilation de l'exécutable (selon l'OS)
ifeq ($(NEED_RAYLIB_BUILD)$(NEED_KOISHI_BUILD),truetrue)
# Windows : raylib et koishi compilés localement
$(TARGET): $(OBJ) $(RAYLIB_LIB) $(KOISHI_LIB)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBPATH) $(LIBS)
else ifeq ($(NEED_RAYLIB_BUILD),true)
# Windows : uniquement raylib compilé
$(TARGET): $(OBJ) $(RAYLIB_LIB)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBPATH) $(LIBS)
else ifeq ($(NEED_KOISHI_BUILD),true)
# Windows : uniquement koishi compilé
$(TARGET): $(OBJ) $(KOISHI_LIB)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBPATH) $(LIBS)
else
# Linux / macOS
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBPATH) $(LIBS)
endif

# Recherche automatique des .c
VPATH = $(sort $(dir $(shell find . -type f -name "*.c") $(shell find ../lib -type f -name "*.c")))

# Règle générique
$(BIN_DIR)/%.o: %.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	$(TARGET)

clean:
	rm -rf $(BIN_DIR)
	mkdir -p $(BIN_DIR)
	# ACTUELLEMENT DESACTIVÉ: supprimer koishi pour forcer la recompilation
	rm -f $(KOISHI_LIB)