# Nom de l'exécutable
TARGET_NAME = jeu
# Dossier de destination (à la racine du projet)
BIN_DIR = ../bin
# Chemin complet de l'exécutable
TARGET = $(BIN_DIR)/$(TARGET_NAME)

CC = gcc

# Recherche des dossiers de header dans ../lib
LIB_DIRS = $(shell find ../lib -type d)
# Inclusion des headers : src/ + tous les sous-dossiers de lib/
INCLUDES = -I. $(addprefix -I,$(LIB_DIRS))

# Flags de compilation
CFLAGS = -Wall -Wextra -g $(INCLUDES)

# Recherche de tous les fichiers .c dans src/ et ../lib/
SRC = $(shell find . -type f -name "*.c") $(shell find ../lib -type f -name "*.c")
# Génération des noms de fichiers objets (.o) correspondants
OBJ = $(SRC:.c=.o)

# La règle par défaut : créer le dossier bin puis l'exécutable
all: $(BIN_DIR) $(TARGET)

# Création du dossier bin s'il n'existe pas
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compilation de l'exécutable dans le dossier bin
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lraylib -lm -lpthread -ldl -lrt -lX11

# Règle générique pour transformer les .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nouvelle directive "run" pour lancer le jeu
run: all
	$(TARGET)

# Nettoyage des fichiers .o et du dossier bin
clean:
	rm -f $(OBJ)
	rm -rf $(BIN_DIR)
