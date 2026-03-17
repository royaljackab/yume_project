bool Player_is_hit(Pool *p, Player entity);
bool Player_is_hit_by_bullet(Pool *p, Player player);
bool Player_is_hit_by_staight_laser(Pool *p, Player player);

bool CheckCircleRotatedRect(Vector2 cPos, float radius,
                            Vector2 rPos, float w, float h, float angle);