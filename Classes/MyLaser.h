#ifndef MY_LASER_H__
#define MY_LASER_H__

#include "cocos2d.h"

class Enemy;

class Laser : public cocos2d::Sprite
{
public:
    Laser(int streak_tag, Enemy* enemy);
    static Laser* spriteWithFile(kMyLaser type, int streak_tag, Enemy* enemy);

    void initStreak(cocos2d::Point point);

    virtual void update(float dt);

    CC_SYNTHESIZE(Enemy*, m_enemy, Enemy);
    CC_SYNTHESIZE(bool, m_canceled, Canceled);
    CC_SYNTHESIZE(int, m_streak_tag, StreakTag);

protected:
    bool m_init;
};

#endif
