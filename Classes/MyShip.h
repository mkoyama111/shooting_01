#ifndef MY_SHIP_H__
#define MY_SHIP_H__

#include <list>
#include "cocos2d.h"
class Laser;

class MyShip : public cocos2d::Sprite
{
public:
    MyShip(kMyBullet bullet, int bullet_damage, int vitality);

    static MyShip* spriteWithFile(const char* pszFileName, kMyBullet bullet, int bullet_damage, int vitality);

    CC_SYNTHESIZE(bool, m_is_touched, IsTouched);
    CC_SYNTHESIZE(cocos2d::Point, m_touch_pos, TouchPos);
    CC_SYNTHESIZE_READONLY(int, m_bullet_damage, BulletDamage);
    CC_SYNTHESIZE_READONLY(int, m_vitality, Vitality);
    CC_SYNTHESIZE_READONLY(int, m_since_damaged, SinceDamaged);

    void update(float dt);

    kMyBullet getBullet() const;
    void setBullet(kMyBullet bullet);

    bool damage(int point);

    void createBullets01();
    void createBullets02();
    void createBullets03();

protected:

    void setLaser();
    void launchLaser();

    void launchLaserCallback(cocos2d::Node* sender, void* target);
    std::list<Laser*> m_laser_list;

    cocos2d::Size m_winSize;
    kMyBullet m_bullet;
    float m_speed_x;
    float m_speed_y;
    int m_duration_of_set_laser;
    int m_my_ship_life_count;
    int m_count;
};

#endif
