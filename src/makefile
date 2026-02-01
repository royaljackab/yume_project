TARGET = jeu
CFLAGS = -Wall

# Recherche récursive de tous les fichiers .c, en excluant ANY ../bin/ ou include/ directories
SRC = $(shell find . -type f -name "*.c" ! -path "*/../bin/*" ! -path "*/include/*")

# Convertir les chemins sources en chemins objets dans le dossier ../bin
OBJ = $(patsubst %.c, ../bin/%.o, $(SRC))

INCLUDE_PATH = include
CFLAGS += -I$(INCLUDE_PATH)

# --- DÉTECTION AUTOMATIQUE DE L'OS ---
# On détecte le système d'exploitation pour ajuster les librairies
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    # CONFIGURATION LINUX
    # On lie raylib, OpenGL, Math, Pthread, et X11/Wayland
    LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
else
    # CONFIGURATION WINDOWS
    # On garde ton ancienne config (assurant que libraylib.a est dans include/ ou lib/)
    LIBS = -Linclude -lraylib -lopengl32 -lgdi32 -lwinmm
endif

all: ../bin/$(TARGET)

../bin/$(TARGET): $(OBJ)
	@echo "Linking..."
	gcc $(OBJ) -o $@ $(LIBS)

# Règle générique pour compiler les fichiers .c en .o
../bin/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	gcc $(CFLAGS) -c $< -o $@

# Créer le dossier ../bin si nécessaire
../bin:
	mkdir -p ../bin

run: all
	./../bin/$(TARGET)

clean:
	rm -rf ../bin/

debug:
	@echo "Sources: $(SRC)"
	@echo "Objects: $(OBJ)"
