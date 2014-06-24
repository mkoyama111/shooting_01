#include "GameConfig.h"
#include "GameLayer.h"
#include "Enemy.h"
#include "MyLaser.h"

USING_NS_CC;

Laser::Laser(int streak_tag, Enemy* enemy) : m_enemy(enemy),  m_canceled(false), m_streak_tag(streak_tag), m_init(false)
{
    auto game_layer = GameLayer::getGameLayer();
    auto laser_lock_layer = (Layer*)game_layer->getChildByTag(GameLayer::kTagLaserLockLayer);

    if (laser_lock_layer) {
        auto lock_mark = Sprite::createWithSpriteFrameName(IMAGE_MY_LASER_LOCK);
        lock_mark->setPosition(enemy->getPosition());
        laser_lock_layer->addChild(lock_mark, GameLayer::kZOrderMyLaserLock, streak_tag);

        if (enemy->getOnBackground()) {
            float move_duration = 0;
            Point move_delta_pos;
            game_layer->getMoveBackgroundParam(move_duration, move_delta_pos);
            auto move = MoveBy::create(move_duration, move_delta_pos);
            lock_mark->runAction(move);
        } else {
            enemy->setLaserLockMark(lock_mark);
        }
    }

}

Laser* Laser::spriteWithFile(kMyLaser type, int streak_tag, Enemy* enemy)
{
    auto pLaser = new Laser(streak_tag, enemy);
    if (pLaser && pLaser->initWithSpriteFrameName(IMAGE_MY_BULLET_LASER)) {

        BlendFunc blend;
        blend.src = GL_SRC_ALPHA;
        blend.dst = GL_ONE;
        pLaser->setBlendFunc(blend);

        pLaser->autorelease();
        pLaser->scheduleUpdate();
        return pLaser;
    }
    CC_SAFE_DELETE(pLaser);
    return NULL;
}

void Laser::initStreak(cocos2d::Point point)
{
    auto game_layer = GameLayer::getGameLayer();
    auto laser_layer = (Layer*)game_layer->getChildByTag(GameLayer::kTagLaserLayer);

    if (laser_layer) {
        MotionStreak* streak = MotionStreak::create(
            MY_LASER_FADE_TIME, 1.0f, MY_LASER_STROKE_WIDTH, Color3B(0xff, 0xff, 0xff), IMAGE_MY_BULLET_LASER);

        BlendFunc blend;
        blend.src = GL_SRC_ALPHA;
        blend.dst = GL_ONE;
        streak->setBlendFunc(blend);

        streak->setPosition(point);
        laser_layer->addChild(streak, GameLayer::kZOrderMyLaserStreak, m_streak_tag);
        m_init = true;
    }
}

void Laser::update(float dt)
{
    if (m_init) {
        auto game_layer = GameLayer::getGameLayer();
        auto laser_layer = (Layer*)game_layer->getChildByTag(GameLayer::kTagLaserLayer);
        if (laser_layer) {
            auto streak = laser_layer->getChildByTag(m_streak_tag);
            if (streak) {
                streak->setPosition(this->getPosition());
            }
        }

    }
}
