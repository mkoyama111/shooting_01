#define NOMINMAX
#include <algorithm>
#include "SimpleAudioEngine.h"
#include "GameConfig.h"
#include "MathUtil.h"
#include "GameLayer.h"
#include "Enemy.h"
#include "MyShip.h"
#include "MyBullet.h"
#include "MyLaser.h"

USING_NS_CC;
using namespace CocosDenshion;

MyShip::MyShip(kMyBullet bullet, int bullet_damage, int vitality) : m_bullet(bullet),
    m_bullet_damage(bullet_damage), m_is_touched(false), m_touch_pos(Point::ZERO),
    m_vitality(vitality), m_duration_of_set_laser(0), m_since_damaged(0), 
    m_my_ship_life_count(MY_SHIP_LIFE_COUNT), m_count(0)
{
    m_winSize = Director::getInstance()->getWinSize();
    m_speed_x = MY_SHIP_SPEED_X;
    m_speed_y = MY_SHIP_SPEED_Y;
}

MyShip* MyShip::spriteWithFile(const char* pszFileName, kMyBullet bullet, int bullet_damage, int vitality)
{
    auto pMyShip = new MyShip(bullet, bullet_damage, vitality);
    if (pMyShip && pMyShip->initWithSpriteFrameName(pszFileName)) {
        pMyShip->autorelease();
        pMyShip->scheduleUpdate();
        return pMyShip;
    }
    CC_SAFE_DELETE(pMyShip);
    return NULL;
}

kMyBullet MyShip::getBullet() const
{
    return m_bullet;
}

void MyShip::setBullet(kMyBullet bullet)
{
    m_bullet = bullet;
    switch (bullet) {
    case kMyBullet_01:
        m_bullet_damage = MY_BULLET_01_DAMAGE;
        break;
    case kMyBullet_02:
        m_bullet_damage = MY_BULLET_02_DAMAGE;
        break;
    case kMyBullet_03:
        m_bullet_damage = MY_BULLET_03_DAMAGE;
        break;
    default:
        break;
    }
}

bool MyShip::damage(int point)
{
    if (m_since_damaged > 0) {
        return false;
    }

    m_vitality -= point;
    this->runAction(CCBlink::create(0.8f, 5));
    this->setOpacity(128);
    m_since_damaged = MY_SHIP_MUTEKI_TIME_AFTER_DAMAGE;

    if (m_vitality <= 0) {
        auto game_layer = GameLayer::getGameLayer();
        m_my_ship_life_count = game_layer->minusMyShipLife();
        m_vitality = MY_SHIP_VITALITY;
        return true;
    }

    return false;
}

void MyShip::update(float dt)
{
    if (m_since_damaged < MY_SHIP_MUTEKI_TIME_AFTER_DAMAGE - 60) {
        if (m_is_touched) {
            Point touch_pos = m_touch_pos;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
            touch_pos.y += 15;
#else
            touch_pos.y += 45;
#endif
            Point pos = getPosition();
            const float diff_x = touch_pos.x - pos.x;
            const float diff_y = touch_pos.y - pos.y;
            if (diff_x > 0.1 || diff_x < -0.1 || diff_y > 0.1 || diff_y < -0.1) {
                const float angle = CC_RADIANS_TO_DEGREES(atan2f((diff_y), (diff_x))) + 360;

                if (touch_pos.x > pos.x) {
                    pos.x = std::min(touch_pos.x, pos.x + m_speed_x * MathUtil::cos(angle));
                } else {
                    pos.x = std::max(touch_pos.x, pos.x + m_speed_x * MathUtil::cos(angle));
                }

                if (touch_pos.y > pos.y) {
                    pos.y = std::min(touch_pos.y, pos.y + m_speed_y * MathUtil::sin(angle));
                } else {
                    pos.y = std::max(touch_pos.y, pos.y + m_speed_y * MathUtil::sin(angle));
                }

                pos.y = std::min(m_winSize.height - getContentSize().height / 2, pos.y);
                pos.y = std::max(CONTROL_AREA_HIGHT + getContentSize().height / 2, pos.y);

                pos.x = std::min(m_winSize.width - getContentSize().width / 2, pos.x);
                pos.x = std::max(getContentSize().width / 2, pos.x);

                this->setPosition(pos);
            }

            if (m_count % 6 == 0) {
                if (m_bullet >= kMyBullet_01) {
                    createBullets01();
                }
                if (m_bullet >= kMyBullet_02) {
                    createBullets02();
                }
                if (m_bullet >= kMyBullet_03) {
                    createBullets03();
                }
            }

            if (m_duration_of_set_laser == 0) {
                setLaser();
            }
        } else {
            launchLaser();
        }
    }

    if (m_duration_of_set_laser > 0) {
        m_duration_of_set_laser--;
    }

    if (m_since_damaged > 0) {
        if (m_my_ship_life_count > 0) {
            if (m_since_damaged >= MY_SHIP_MUTEKI_TIME_AFTER_DAMAGE - 60 &&
                m_since_damaged < MY_SHIP_MUTEKI_TIME_AFTER_DAMAGE) {
                Point pos = getPosition();
                pos.y += 1;
                this->setPosition(pos);
            }
            if (m_since_damaged == 1) {
                this->setOpacity(255);
            }
            m_since_damaged--;
        }
    }

    m_count++;
}

void MyShip::createBullets01()
{
    auto game_layer = GameLayer::getGameLayer();

    const int x = this->getPosition().x;
    const int y = this->getPosition().y;

    auto my_bullet1 = MyBullet::spriteWithFile(kMyBullet_01, 90,  MY_BULLET_01_SPEED, 0);
    my_bullet1->setPosition(Point(x - 5, y));
    game_layer->addChild(my_bullet1, GameLayer::kZOrderMyBullet, GameLayer::kTagMyBullet);

    auto my_bullet2 = MyBullet::spriteWithFile(kMyBullet_01, 90,  MY_BULLET_01_SPEED, 0);
    my_bullet2->setPosition(Point(x + 5, y));
    game_layer->addChild(my_bullet2, GameLayer::kZOrderMyBullet, GameLayer::kTagMyBullet);
}

void MyShip::createBullets02()
{
    auto game_layer = GameLayer::getGameLayer();

    const int x = this->getPosition().x;
    const int y = this->getPosition().y;

    auto my_bullet1 = MyBullet::spriteWithFile(kMyBullet_02, 90,  MY_BULLET_02_SPEED, 0);
    my_bullet1->setPosition(Point(x - 15, y));
    game_layer->addChild(my_bullet1, GameLayer::kZOrderMyBullet, GameLayer::kTagMyBullet);

    auto my_bullet2 = MyBullet::spriteWithFile(kMyBullet_02, 90,  MY_BULLET_02_SPEED, 0);
    my_bullet2->setPosition(Point(x + 15, y));
    game_layer->addChild(my_bullet2, GameLayer::kZOrderMyBullet, GameLayer::kTagMyBullet);
}


void MyShip::createBullets03()
{
    auto game_layer = GameLayer::getGameLayer();

    auto my_bullet1 = MyBullet::spriteWithFile(kMyBullet_03, 75/*72*/,  MY_BULLET_03_SPEED, 0);
    my_bullet1->setPosition(this->getPosition());
    game_layer->addChild(my_bullet1, GameLayer::kZOrderMyBullet, GameLayer::kTagMyBullet);

    auto my_bullet2 = MyBullet::spriteWithFile(kMyBullet_03, 105/*108*/,  MY_BULLET_03_SPEED, 0);
    my_bullet2->setPosition(this->getPosition());
    game_layer->addChild(my_bullet2, GameLayer::kZOrderMyBullet, GameLayer::kTagMyBullet);
}

void MyShip::setLaser()
{
    auto game_layer = GameLayer::getGameLayer();

    for (auto elem : m_laser_list) {
        auto laser = elem;
        if (laser->getCanceled()) {
            continue;
        }
        auto enemy = laser->getEnemy();
        if (enemy->getFinished()) {
            auto laser_lock_layer = static_cast<Layer*>(game_layer->getChildByTag(GameLayer::kTagLaserLockLayer));
            if (laser_lock_layer) {
                auto lock_mark = laser_lock_layer->getChildByTag(laser->getStreakTag());
                lock_mark->setVisible(false);
            }
            laser->setCanceled(true);
        }
    }

    auto laser_layer = static_cast<Layer*>(game_layer->getChildByTag(GameLayer::kTagLaserLayer));
    if (laser_layer) {
        auto& enemy_list = game_layer->getEnemyList();
        int i = 0;

        for (auto elem : enemy_list) {
        #if 1
            // 地上のオブジェクトのみをロックする
            if (!elem->getOnBackground() || elem->getFinished() || elem->getLaserLocked()) {
        #else
            if (elem->getFinished() || elem->getLaserLocked()) {
        #endif
                continue;
            }
            float dist = elem->getPosition().getDistance(this->getPosition());
            if (dist < MY_LASER_LOCKING_DISTANCE) {
                elem->setLaserLocked(true);
                SimpleAudioEngine::getInstance()->playEffect(SE_MY_LASER_LOCK);
                const int tag = m_count + i;
                auto laser = Laser::spriteWithFile(kMyLaser_01, tag, elem);
                laser_layer->addChild(laser, GameLayer::kZOrderMyBullet);
                m_laser_list.push_back(laser);
                i++;
            }
        }
    }
}

void MyShip::launchLaser()
{
    for (auto elem : m_laser_list) {
        auto laser = elem;
        auto enemy = laser->getEnemy();
        if (enemy->getFinished() || laser->getCanceled()) {

            auto game_layer = GameLayer::getGameLayer();
            auto laser_lock_layer = static_cast<Layer*>(game_layer->getChildByTag(GameLayer::kTagLaserLockLayer));
            if (laser_lock_layer) {
                auto lock_mark = laser_lock_layer->getChildByTag(laser->getStreakTag());
                if (lock_mark) {
                    lock_mark->removeFromParentAndCleanup(true);
                }
            }
            laser->removeFromParentAndCleanup(true);

        } else {
            laser->setPosition(this->getPosition());
            laser->initStreak(this->getPosition());

            auto p_array = PointArray::create(6);
            {
                const float dx = (this->getPosition().x > enemy->getPosition().x) ? 5.0f : -5.0f;
                const float dy = (this->getPosition().y > enemy->getPosition().y) ? 250.0f : -250.0f;
                const float dy2 = 90.0f;
                p_array->addControlPoint(this->getPosition());
                p_array->addControlPoint(Point(this->getPosition().x + dx, this->getPosition().y + dy));
                p_array->addControlPoint(Point(this->getPosition().x + dx, this->getPosition().y));
                p_array->addControlPoint(Point(enemy->getPosition().x - dx, enemy->getPosition().y));
                p_array->addControlPoint(Point(enemy->getPosition().x - dx , enemy->getPosition().y));
                p_array->addControlPoint(Point(enemy->getPosition().x, enemy->getPosition().y - dy2));
            }
            auto spline = CardinalSplineTo::create(MY_LASER_SPLINE_DURATION, p_array, 0.0f);

            auto funcN = CallFuncN::create(CC_CALLBACK_1(MyShip::launchLaserCallback, this, reinterpret_cast<void*>(enemy)));
            auto remove = RemoveSelf::create();
            auto seq = Sequence::create(spline, funcN, remove, NULL);

            laser->runAction(seq);

            m_duration_of_set_laser = DURATION_OF_SET_MY_LASER;
        }
    }
    m_laser_list.clear();
}

void MyShip::launchLaserCallback(cocos2d::Node* sender, void* target)
{
    auto game_layer = GameLayer::getGameLayer();

    auto enemy = reinterpret_cast<Enemy*>(target);
    if (enemy) {
        if (!enemy->getFinished()) {
            if (enemy->damage(MY_LASER_DAMAGE)) {
                game_layer->addScore(enemy->getScore());
                game_layer->createExplosion(static_cast<GameLayer::kTag>(enemy->getTag()), enemy->getPosition(), true);
            } else {
                game_layer->addScore(10);
            }
        }
        enemy->setLaserLocked(false);
    }

    auto laser = static_cast<Laser*>(sender);

    auto laser_lock_layer = static_cast<Layer*>(game_layer->getChildByTag(GameLayer::kTagLaserLockLayer));
    if (laser_lock_layer) {
        auto lock_mark = laser_lock_layer->getChildByTag(laser->getStreakTag());
        if (lock_mark) {
            lock_mark->removeFromParentAndCleanup(true);
        }
    }

    auto laser_layer = static_cast<Layer*>(game_layer->getChildByTag(GameLayer::kTagLaserLayer));
    if (laser_layer) {
        auto streak = laser_layer->getChildByTag(laser->getStreakTag());
        if (streak) {
            streak->removeFromParentAndCleanup(true);
        }
    }
}
