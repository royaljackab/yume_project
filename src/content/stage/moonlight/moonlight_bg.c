#include "moonlight_bg.h"
#include "assets.h"
#include "screen.h"
#include "sprite.h"
#include <raylib.h>
#include <rlgl.h>

static Camera3D bg_camera;
static float corridor_z = 0;
static float look_at_x = 0;

static bool current_is_spellcard = false;
static bool next_is_spellcard = false;
static bool in_transition = false;
static float transition_alpha = 0;
static const float TRANSITION_SPEED = 0.1;

static
void DrawSprite3D(Texture2D texture, Rectangle source, Vector3 position, float width, float height, Vector3 rotationAxis, float rotationAngle, Color color) {
    rlCheckRenderBatchLimit(4);
    rlSetTexture(texture.id);
    rlPushMatrix();
    rlTranslatef(position.x, position.y, position.z);
    rlRotatef(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);

    float tx = source.x / (float)texture.width;
    float ty = source.y / (float)texture.height;
    float tw = source.width / (float)texture.width;
    float th = source.height / (float)texture.height;

    rlBegin(RL_QUADS);
        rlColor4ub(color.r, color.g, color.b, color.a);
        rlNormal3f(0, 0, 1);
        rlTexCoord2f(tx, ty + th);      rlVertex3f(-width/2, -height/2, 0);
        rlTexCoord2f(tx + tw, ty + th); rlVertex3f(width/2, -height/2, 0);
        rlTexCoord2f(tx + tw, ty);      rlVertex3f(width/2, height/2, 0);
        rlTexCoord2f(tx, ty);           rlVertex3f(-width/2, height/2, 0);
    rlEnd();
    rlPopMatrix();
    rlSetTexture(0);
}

void moonlight_bg_init() {
    bg_camera.position = (Vector3){0,15,10};
    bg_camera.target = (Vector3){0, 5,0};
    bg_camera.up = (Vector3){0,1,0};
    bg_camera.fovy = 60;
    bg_camera.projection = CAMERA_PERSPECTIVE;

    corridor_z = 0;
    look_at_x = 0;

    current_is_spellcard = false;
    next_is_spellcard = false;
    in_transition = false;
    transition_alpha = 0;
}

void moonlight_bg_set_mode(bool to_spellcard) {
    if (current_is_spellcard != to_spellcard && !in_transition) {
        next_is_spellcard = to_spellcard;
        in_transition = true;
    }
}

void moonlight_bg_update(int frames) {

    if (!current_is_spellcard) {
        // On avance dans le corridorrrr
        corridor_z += 0.2;
        if (corridor_z > 40) corridor_z -= 40;
        
        look_at_x = sinf((float)frames * 0.001f) * 6.0f + 5; 
        
        bg_camera.target.x = look_at_x;
    }

    if (in_transition) {
        transition_alpha += TRANSITION_SPEED;

        if (transition_alpha >= 1.0) {
            transition_alpha = 1;
            current_is_spellcard = next_is_spellcard;
            in_transition = false;
        }
    } else if (transition_alpha > 0) {
        transition_alpha -= TRANSITION_SPEED;
        if (transition_alpha < 0) {
            transition_alpha = 0;
        }
    }
    
}

void moonlight_bg_draw(GameContext *ctx) {
    // 2 choix : spellcard alors mode 2D en scroll
    // Nonspell alors fond en 3d
    if (current_is_spellcard) {
        Sprite_draw_range(ctx->pool, -50, -1);
    } else {
        Texture2D stg4_tex = textures[TH14_STG4];

        Rectangle rect_floor = {1, 322, 1024, 1024};
        Rectangle rect_wall = {1, 1, 512, 320};

        BeginMode3D(bg_camera);
            for (int i=0; i < 6; i++) {
                float z = -(i * 40) + corridor_z;

                DrawSprite3D(stg4_tex, rect_floor, (Vector3){0,0,z}, 40, 40, (Vector3){1,0,0}, -90, WHITE);
                DrawSprite3D(stg4_tex, rect_wall, (Vector3){20,10,z}, 40, 20, (Vector3){0,1,0}, -90, WHITE);
                DrawSprite3D(stg4_tex, rect_wall, (Vector3){-20,10,z}, 40, 20, (Vector3){0,1,0}, 90, WHITE);
            }
        EndMode3D();
    }

    if (transition_alpha > 0) {
        Color fade_color = Fade(BLACK, transition_alpha);
        DrawRectangle(PANEL_LEFT, PANEL_UP, PANEL_WIDTH, PANEL_HEIGHT, fade_color);
    }
}