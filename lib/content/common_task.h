#pragma once

#include "coroutine/tasks.h"

DECLARE_EXTERN_TASK(phase_timer, { CoEvent *event; int duration; });

DECLARE_EXTERN_TASK(obj_GoTo, {Pool *pool; Entity objId; float x; float y; float speed;});

DECLARE_EXTERN_TASK(play_anim_once, {Pool *pool; float x; float y; int sprite_id; });

DECLARE_EXTERN_TASK(boss_pentagram_effect, {Pool *pool; Entity boss; });
DECLARE_EXTERN_TASK(boss_particle_spawner, {Pool *pool; Entity boss; });