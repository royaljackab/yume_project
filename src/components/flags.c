#include "components/flags.h"
#include <stdio.h>

/**
 * @brief Affiche le type de flag dans la console (debug)
 * @param flag Pointeur vers le composant Flag
 */
void Flag_display(Flag *flag) {
    if (!flag) {
        printf("Flag: NULL\n");
        return;
    }

    const char *type_str;
    switch (flag->type) {
        case FLAG_NONE:           type_str = "NONE"; break;
        case FLAG_PLAYER:         type_str = "PLAYER"; break;
        case FLAG_ENEMY:          type_str = "ENEMY"; break;
        case FLAG_BOSS:           type_str = "BOSS"; break;
        case FLAG_BULLET_PLAYER:  type_str = "BULLET_PLAYER"; break;
        case FLAG_BULLET_ENEMY:   type_str = "BULLET_ENEMY"; break;
        case FLAG_POWERUP:        type_str = "POWERUP"; break;
        case FLAG_WALL:           type_str = "WALL"; break;
        default:                  type_str = "UNKNOWN"; break;
    }
    printf("Flag: [Type: %s]\n", type_str);
}