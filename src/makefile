TARGET = jeu
CFLAGS = -Wall

# Recherche récursive de tous les fichiers .c, en excluant ANY ../bin/ ou include/ directories
SRC = $(shell find . -type f -name "*.c" ! -path "*/../bin/*" ! -path "*/include/*")

# Convertir les chemins sources en chemins objets dans le dossier ../bin
OBJ = $(patsubst %.c, ../bin/%.o, $(SRC))

INCLUDE_PATH = include
CFLAGS += -I$(INCLUDE_PATH)

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
    LIBS = -L$(INCLUDE_PATH) -lraylib -lopengl32 -lgdi32 -lwinmm
endif


all: build/$(TARGET)

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
