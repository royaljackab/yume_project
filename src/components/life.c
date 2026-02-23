#include "life.h"
#include "pool.h"

bool hasZeroLife(Life *life) {
    /***
     * Retourne si le composant life a une vie de 0 ou moins
     */
    return life->hp <= 0;
}

