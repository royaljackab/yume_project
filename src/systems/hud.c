/**
 * @file hud.c
 * @brief Implémentation du HUD (Heads-Up Display)
 *
 * @author Amy
 */

#include "systems/hud.h"
#include "core/screen.h"
#include "components/player.h"
#include "ecs/pool.h"

#include <raylib.h>
#include <stdio.h>

/** @brief Position x du cadre HUD */
#define HUD_X (PANEL_RIGHT + 40)
/** @brief Position y du cadre HUD */
#define HUD_Y (PANEL_UP + 20)
/** @brief Largeur du cadre HUD */
#define HUD_WIDTH 350
/** @brief Hauteur du cadre HUD */
#define HUD_HEIGHT 250
/** @brief Marge intérieure du cadre HUD */
#define HUD_PADDING 15

/** @brief Couleur de fond du HUD (semi-transparent) */
#define HUD_BG       (Color){0, 0, 0, 180}
/** @brief Couleur de la bordure du HUD */
#define HUD_BORDER   WHITE
/** @brief Couleur du titre */
#define HUD_TITLE_COL WHITE
/** @brief Couleur des labels */
#define HUD_LABEL_COL GRAY
/** @brief Couleur des valeurs */
#define HUD_VALUE_COL WHITE
/** @brief Couleur des vies */
#define HUD_LIFE_COL  RED
/** @brief Couleur des bombes */
#define HUD_BOMB_COL  YELLOW

void HUD_draw(GameContext *ctx, const char *stage_name) {
    Pool *p = ctx->pool;

    /* Récupérer les infos du joueur */
    Entity player_entity = p->player.entity_lookup[0];
    Player *player = Player_get(&p->player, player_entity);
    if (!player) return;

    int x = HUD_X;
    int y = HUD_Y;

    /* Fond du cadre */
    DrawRectangle(x, y, HUD_WIDTH, HUD_HEIGHT, HUD_BG);
    DrawRectangleLines(x, y, HUD_WIDTH, HUD_HEIGHT, HUD_BORDER);

    /* Contenu à l'intérieur du cadre */
    int cx = x + HUD_PADDING;
    int cy = y + HUD_PADDING;

    /* Titre du stage */
    DrawText(stage_name, cx, cy, 20, HUD_TITLE_COL);
    cy += 25;
    DrawLine(cx, cy, cx + HUD_WIDTH - (HUD_PADDING * 2), cy, HUD_BORDER);
    cy += 15;

    /* Score */
    char score_text[30];
    sprintf(score_text, "SCORE   %09d", 0); /* TODO: vrai score */
    DrawText(score_text, cx, cy, 20, HUD_VALUE_COL);
    cy += 35;

    /* Vies : cercles rouges pleins si vie restante, contour sinon */
    DrawText("LIVES", cx, cy, 18, HUD_LABEL_COL);
    for (int i = 0; i < player->nbLives; i++) {
        int lx = cx + 80 + (i * 28);
        if (i < player->lives) {
            DrawCircle(lx, cy + 9, 9, HUD_LIFE_COL);
        } else {
            DrawCircleLines(lx, cy + 9, 9, HUD_LIFE_COL);
        }
    }
    cy += 35;

    /* Bombes : carrés jaunes pleins si bombe restante, contour sinon */
    DrawText("BOMBS", cx, cy, 18, HUD_LABEL_COL);
    for (int i = 0; i < player->nbBombs; i++) {
        int bx = cx + 80 + (i * 28);
        if (i < player->bombs) {
            DrawRectangle(bx - 8, cy, 16, 16, HUD_BOMB_COL);
        } else {
            DrawRectangleLines(bx - 8, cy, 16, 16, HUD_BOMB_COL);
        }
    }
}