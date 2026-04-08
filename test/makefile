CC = gcc
CFLAGS = -Wall -Wextra -g -I../lib -I../lib/core -I../lib/components -I../lib/systems -I../lib/ecs -I/opt/homebrew/include -I../raylib/src -L/opt/homebrew/lib -lraylib

SRC_LIFE      = ../src/components/life.c
SRC_PHYSICS   = ../src/components/physics.c ../src/components/common.c
SRC_COLLISION = ../src/systems/collision_entity.c

all: test_life test_physics test_collision

test_life: test_life.c $(SRC_LIFE)
	$(CC) $(CFLAGS) -o test_life test_life.c $(SRC_LIFE)

test_physics: test_physics.c
	$(CC) $(CFLAGS) -o test_physics test_physics.c

test_collision: test_collision.c $(SRC_COLLISION)
	$(CC) $(CFLAGS) -o test_collision test_collision.c $(SRC_COLLISION)

run: all
	@echo "=== Tests unitaires Moonlight ==="
	@./test_life
	@./test_physics
	@./test_collision
	@echo "=== Fin des tests ==="

clean:
	rm -f test_life test_physics test_collision
	rm -rf *.dSYM