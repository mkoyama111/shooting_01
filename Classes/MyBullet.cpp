#include "GameConfig.h"
#include "GameLayer.h"
#include "MyBullet.h"
#include "MathUtil.h"

USING_NS_CC;

MyBullet::MyBullet(int angle, float speed, float accel) : m_angle(angle), m_speed(speed), m_accel(accel)
{
    m_winSize = Director::getInstance()->getWinSize();
    m_dx = speed * MathUtil::cos(angle);
    m_dy = speed * MathUtil::sin(angle);

    this->setRotation(360.0f - m_angle + 90.0f);
}

MyBullet* MyBullet::spriteWithFile(kMyBullet type, int angle, float speed, float accel)
{
    std::string sprite_frame_name;
    switch (type) {
    case kMyBullet_01:
        sprite_frame_name = std::string(IMAGE_MY_BULLET_01);
        break;
    case kMyBullet_02:
        sprite_frame_name = std::string(IMAGE_MY_BULLET_02);
        break;
    case kMyBullet_03:
        sprite_frame_name = std::string(IMAGE_MY_BULLET_03);
        break;
    default:
        break;
    }

    auto pMyBullet = new MyBullet(angle, speed, accel);

    if (pMyBullet && pMyBullet->initWithSpriteFrameName(sprite_frame_name.c_str())) {
        pMyBullet->autorelease();
        pMyBullet->scheduleUpdate();
        return pMyBullet;
    }
    CC_SAFE_DELETE(pMyBullet);
    return NULL;
}

void MyBullet::update(float dt)
{
    Point pos = this->getPosition();

    pos.x += m_dx;
    pos.y += m_dy;

    if (pos.x > m_winSize.width + getContentSize().width ||
        pos.x < -getContentSize().width ||
        pos.y > m_winSize.height + getContentSize().height) {
        this->removeFromParentAndCleanup(true);
    } else {
        this->setPosition(pos);
    }
}
