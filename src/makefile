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
    LIBPATH =   -L../lib # pas de -L nécessaire si raylib est installé dans le système
    NEED_RAYLIB_BUILD = false
else ifeq ($(UNAME_S),Darwin)
    # macOS (exemple, à adapter selon votre installation)
    LIBS = -lraylib -lm -lpthread -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -lkoishi
    LIBPATH = -L/opt/homebrew/lib -L../lib
    NEED_RAYLIB_BUILD = false
else
    # Windows (MSYS2, MinGW)
    LIBS = -lraylib -lm -lpthread -lopengl32 -lgdi32 -lwinmm -lkoishi
    LIBPATH = -L../include/src -L../lib
    NEED_RAYLIB_BUILD = true
endif

# Recherche des dossiers de header dans ../lib
LIB_DIRS = $(shell find ../lib -type d)
# Inclusion des headers : src/ + tous les sous-dossiers de lib/ + raylib include
INCLUDES = -I. $(addprefix -I,$(LIB_DIRS)) -I../include -I../include/src -I../raylib/src

# Flags de compilation
CFLAGS = -g $(INCLUDES)

# Recherche de tous les fichiers .c dans src/ et ../lib/
SRC = $(shell find . -type f -name "*.c") $(shell find ../lib -type f -name "*.c")
# Génération des noms de fichiers objets (.o) dans le dossier bin
OBJ = $(addprefix $(BIN_DIR)/, $(notdir $(SRC:.c=.o)))

# Sous Windows, on vérifie si la bibliothèque raylib est déjà compilée
ifeq ($(NEED_RAYLIB_BUILD),true)
RAYLIB_A = ../include/src/libraylib.a
$(RAYLIB_A):
	@echo "Raylib library not found, compiling it..."
	cd ../raylib/src && make PLATFORM=PLATFORM_DESKTOP
else
RAYLIB_A = 
endif

# La règle par défaut : créer le dossier bin puis l'exécutable
all: $(BIN_DIR) $(TARGET)

# Création du dossier bin s'il n'existe pas
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compilation de l'exécutable dans le dossier bin
$(TARGET): $(OBJ) $(RAYLIB_A)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBPATH) $(LIBS)

# Recherche automatique des .c dans tous les sous-dossiers (src/ et lib/)
VPATH = $(sort $(dir $(shell find . -type f -name "*.c") $(shell find ../lib -type f -name "*.c")))

# Règle générique : compile un .c et place le .o dans bin/
$(BIN_DIR)/%.o: %.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Nouvelle directive "run" pour lancer le jeu
run: all
	$(TARGET)

# Nettoyage des fichiers .o et du dossier bin
clean:
	rm -rf $(BIN_DIR)