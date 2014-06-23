#ifndef MY_BULLET_H__
#define MY_BULLET_H__

#include "cocos2d.h"

class MyBullet : public cocos2d::CCSprite
{
public:
    MyBullet(int angle, float speed, float accel);

    static MyBullet* spriteWithFile(kMyBullet type, int angle, float speed, float accel);

    void update(float dt);

protected:
    cocos2d::Size m_winSize;
    int m_angle;
    float m_speed;
    float m_accel;
    float m_dx;
    float m_dy;
};

#endif
