#ifndef ENEMY_BULLET_H__
#define ENEMY_BULLET_H__

#include "cocos2d.h"

class EnemyBullet : public cocos2d::Sprite
{
public:
    EnemyBullet(float x, float y, int angle, float speed, float accel, float speed_end, int stop_time, int move_time,
        int stop_time_2, int angle_2, float speed_2, float accel_2, float speed_end_2, int vanish_time);

    CC_SYNTHESIZE_READONLY(bool, m_penetrate, Penetrate);

    static EnemyBullet* spriteWithFile(kEnemyBullet type, float x, float y, int angle, float speed,
            float accel, float speed_end, int stop_time, int move_time, int stop_time_2,
            int angle_2, float speed_2, float accel_2, float speed_end_2, int vanish_time);

    void update(float dt);

protected:
    cocos2d::Size m_winSize;
    int m_angle;
    float m_speed;
    float m_accel;
    float m_speed_end;
    float m_dx;
    float m_dy;
    int m_stop_time;
    int m_move_time;
    int m_stop_time_2;
    int m_angle_2;
    float m_speed_2;
    float m_accel_2;
    float m_speed_end_2;
    int m_vanish_time;
};

#endif
