#include "components/life.h"

void Life_damage(Life *life, int damage) { life->life -= damage; }

void Life_heal(Life *life, int heal) { life->life += heal; }

bool Life_is_dead(Life *life) { return life->life <= 0; }
