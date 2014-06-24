#include "GameConfig.h"
#include "GameLayer.h"
#include "EnemyBullet.h"
#include "MathUtil.h"

USING_NS_CC;

EnemyBullet::EnemyBullet(float x, float y, int angle, float speed, float accel, float speed_end, int stop_time, int move_time,
    int stop_time_2, int angle_2, float speed_2, float accel_2, float speed_end_2, int vanish_time) :
        m_angle(angle), m_speed(speed), m_accel(accel), m_speed_end(speed_end), m_penetrate(false),
        m_stop_time(stop_time), m_move_time(move_time), m_stop_time_2(stop_time_2), 
        m_angle_2(angle_2), m_speed_2(speed_2), m_accel_2(accel_2), m_speed_end_2(speed_end_2),
        m_vanish_time(vanish_time)
{
    m_winSize = Director::getInstance()->getWinSize();
    m_dx = speed * MathUtil::cos(angle);
    m_dy = speed * MathUtil::sin(angle);
    this->setPosition(Point(x, y));
}

EnemyBullet* EnemyBullet::spriteWithFile(kEnemyBullet type, float x, float y, int angle, float speed,
    float accel, float speed_end, int stop_time, int move_time, int stop_time_2,
    int angle_2, float speed_2, float accel_2, float speed_end_2, int vanish_time)
{
    auto pEnemyBullet = new EnemyBullet(x, y, angle, speed, accel, speed_end, stop_time, move_time, stop_time_2,
        angle_2, speed_2, accel_2, speed_end_2, vanish_time);

    if (!pEnemyBullet) {
        return NULL;
    }
    bool ret = false;
    if (type == kEnemyBullet_01) {
        ret = pEnemyBullet->initWithSpriteFrameName(IMAGE_ENEMY_BULLET_01);
    } else if (type == kEnemyBullet_02) {
        ret = pEnemyBullet->initWithSpriteFrameName(IMAGE_ENEMY_BULLET_02);
    } else if (type == kEnemyBullet_03) {
        ret = pEnemyBullet->initWithSpriteFrameName(IMAGE_ENEMY_BULLET_03);
    } else if (type == kEnemyBullet_04) {
        ret = pEnemyBullet->initWithSpriteFrameName(IMAGE_ENEMY_BULLET_04);
    } else if (type == kEnemyBullet_05) {
        ret = pEnemyBullet->initWithSpriteFrameName(IMAGE_ENEMY_BULLET_05);
        pEnemyBullet->m_penetrate = true;
        BlendFunc blend;
        blend.src = GL_SRC_ALPHA;
        blend.dst = GL_ONE;
        pEnemyBullet->setBlendFunc(blend);
    } else if (type == kEnemyBullet_06) {
        ret = pEnemyBullet->initWithSpriteFrameName(IMAGE_ENEMY_BULLET_06);
    }
    if (ret) {
        pEnemyBullet->setRotation(360.0f - angle + 90.0f);
        pEnemyBullet->autorelease();
        pEnemyBullet->scheduleUpdate();
        return pEnemyBullet;
    }
    CC_SAFE_DELETE(pEnemyBullet);
    return NULL;
}

void EnemyBullet::update(float dt)
{
    if (m_stop_time > 0) {
        m_stop_time--;
        return;
    }
    if (m_move_time == 0 && m_stop_time_2 > 0) {
        m_stop_time_2--;
        if (m_stop_time_2 == 0) {
            if (m_angle_2 != -1) {
                m_angle = m_angle_2;
            }
            if (m_speed_2 != -1) {
                m_speed = m_speed_2;
            }
            if (m_accel_2 != -1) {
                m_accel = m_accel_2;
            }
            if (m_speed_end_2 != -1) {
                m_speed_end = m_speed_end_2;
            }
            m_dx = m_speed * MathUtil::cos(m_angle);
            m_dy = m_speed * MathUtil::sin(m_angle);
        }
        return;
    }

    Point pos = this->getPosition();

    pos.x += m_dx;
    pos.y += m_dy;

    if (pos.x > m_winSize.width + getContentSize().width * 2 ||
        pos.x < -getContentSize().width * 2 ||
        pos.y > m_winSize.height + getContentSize().height * 2 ||
        pos.y < CONTROL_AREA_HIGHT - getContentSize().height * 2) {
        this->removeFromParentAndCleanup(true);
        return;
    }

    this->setPosition(pos);

    if (m_accel != 0) {
        if ((m_accel > 0 && m_speed < m_speed_end) || (m_accel < 0 && m_speed > m_speed_end)) {
            m_speed += m_accel;
            m_dx = m_speed * MathUtil::cos(m_angle);
            m_dy = m_speed * MathUtil::sin(m_angle);
        }
    }

    if (m_move_time > 0) {
        m_move_time--;
    }

    if (m_vanish_time > 0) {
        m_vanish_time--;
        if (m_vanish_time == 0) {
            this->removeFromParentAndCleanup(true);
        }
    }
}
