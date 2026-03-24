CC = gcc
CFLAGS = -Wall -Wextra -g -I../lib -I../lib/core -I../lib/components -I../lib/systems -I../lib/ecs -I/opt/homebrew/include -I../raylib/src -L/opt/homebrew/lib -lraylib

SRC_LIFE    = ../src/components/life.c
SRC_PHYSICS = ../src/components/physics.c ../src/components/common.c

all: test_life test_physics

test_life: test_life.c $(SRC_LIFE)
	$(CC) $(CFLAGS) -o test_life test_life.c $(SRC_LIFE)


test_physics: test_physics.c
	$(CC) $(CFLAGS) -o test_physics test_physics.c

run: all
	@echo "=== Tests unitaires Moonlight ==="
	@./test_life
	@./test_physics
	@echo "=== Fin des tests ==="

clean:
	rm -f test_life test_physics
	rm -rf *.dSYM