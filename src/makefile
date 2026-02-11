TARGET = jeu
CFLAGS = -Wall

# Recherche récursive de tous les fichiers .c dans src/
SRC = $(shell find . -type f -name "*.c" ! -path "../bin/*")

# Convertir les chemins sources en chemins objets dans le dossier ../bin
OBJ = $(patsubst %.c, ../bin/%.o, $(SRC))

# --- CHEMINS D'INCLUDE ---
# Ajouter tous les répertoires qui contiennent des .h depuis ../lib/
LIB_DIR = ../lib
RAYLIB_INCLUDE = ../raylib/src  # Ajout du chemin vers raylib.h
INCLUDE_DIRS = $(LIB_DIR) \
               $(LIB_DIR)/components \
               $(LIB_DIR)/content \
               $(LIB_DIR)/content/stages \
               $(LIB_DIR)/content/ui \
               $(LIB_DIR)/core \
               $(LIB_DIR)/ecs \
               $(LIB_DIR)/systems \
               $(RAYLIB_INCLUDE)

CFLAGS += $(addprefix -I, $(INCLUDE_DIRS))

# --- DÉTECTION AUTOMATIQUE DE L'OS ---
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    # CONFIGURATION LINUX
    LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
else ifeq ($(UNAME_S),Darwin)
    # CONFIGURATION MACOS (:p)
    # On utilise les frameworks natifs d'Apple pour OpenGL
    LIBS = -L/opt/homebrew/lib -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
else
    # CONFIGURATION WINDOWS
    LIBS = -L$(LIB_DIR)/../include -lraylib -lopengl32 -lgdi32 -lwinmm
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
	@echo "Include dirs: $(INCLUDE_DIRS)"
	@echo "Lib dir: $(LIB_DIR)"
