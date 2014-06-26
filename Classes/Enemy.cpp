#define NOMINMAX
#include <algorithm>
#include <cmath>
#include "GameConfig.h"
#include "GameLayer.h"
#include "Enemy.h"
#include "EnemyBullet.h"
#include "MathUtil.h"

USING_NS_CC;

#define DEF_ENEMY(X) \
    class X : public Enemy { \
    public: \
        X(kEnemy type, int num, float x, float y, int angle, int vitality, int score, bool on_bg, kItem item) : \
            Enemy(type, num, x, y, angle, vitality, score, on_bg, item) {} \
    protected: \
        virtual void update2(); \
    }

DEF_ENEMY(Enemy01);
DEF_ENEMY(Enemy02);
DEF_ENEMY(Enemy03);
DEF_ENEMY(Enemy04);
DEF_ENEMY(Enemy05);
DEF_ENEMY(Enemy06);
DEF_ENEMY(Enemy07);
DEF_ENEMY(Enemy08);
DEF_ENEMY(Enemy09);
DEF_ENEMY(EnemyGround01);
DEF_ENEMY(EnemyBoss01);
DEF_ENEMY(EnemyItem01);

Enemy::Enemy(kEnemy type, int num, float x, float y, int angle, int vitality, int score, bool on_bg, kItem item) :
    m_finished(false), m_enemy_type(type), m_vitality(vitality), m_score(score), m_on_background(on_bg), m_is_item(false), 
    m_shadow(NULL), m_laser_locked(false), m_laser_lock_mark(NULL), m_num(num), m_angle(angle), m_angle_f(angle),
    m_speed(0), m_accel(0), m_count(0), m_dx(0), m_dy(0), m_item(item)
{
    m_winSize = Director::getInstance()->getWinSize();
    this->setPosition(Point(x, y));

    if (on_bg) {
        auto game_layer = GameLayer::getGameLayer();
        float move_duration = 0;
        Point move_delta_pos;
        game_layer->getMoveBackgroundParam(move_duration, move_delta_pos);
        const float n_winSize = (move_delta_pos.y > 0) ? move_delta_pos.y / m_winSize.height : -move_delta_pos.y / m_winSize.height;
        move_delta_pos.y /= n_winSize;
        move_duration /= n_winSize;
        auto move = MoveBy::create(move_duration, move_delta_pos);
        auto pfunc = CallFunc::create(CC_CALLBACK_0(Enemy::FinishSelf, this));
        this->runAction(Sequence::create(move, pfunc, NULL));
    }

    this->setRotation(360.0f - m_angle + 90.0f);
}

Enemy::~Enemy()
{
    auto game_layer = GameLayer::getGameLayer();
    if (game_layer) {
        if (m_shadow) {
            m_shadow->removeFromParentAndCleanup(true);
        }
    }
}

Enemy* Enemy::createEnemy(kEnemy type, int num, float x, float y, int angle, int vitality, int score, bool on_bg, kItem item)
{
    return spriteWithFile(type, num, x, y, angle, vitality, score, on_bg, item);
}

Enemy* Enemy::spriteWithFile(kEnemy type, int num, float x, float y, int angle, int vitality, int score, bool on_bg, kItem item)
{
    Enemy* pEnemy = NULL;
    std::string sprite_frame_name;
    switch (type) {
    case kEnemy_01:
        pEnemy = new Enemy01(type, num, x, y, angle, vitality, score, on_bg, item);
        sprite_frame_name = std::string(IMAGE_ENEMY_01);
        break;
    case kEnemy_02:
        pEnemy = new Enemy02(type, num, x, y, angle, vitality, score, on_bg, item);
        sprite_frame_name = std::string(IMAGE_ENEMY_02);
        break;
    case kEnemy_03:
        pEnemy = new Enemy03(type, num, x, y, angle, vitality, score, on_bg, item);
        sprite_frame_name = std::string(IMAGE_ENEMY_03);
        break;
    case kEnemy_04:
        pEnemy = new Enemy04(type, num, x, y, angle, vitality, score, on_bg, item);
        sprite_frame_name = std::string(IMAGE_ENEMY_04);
        break;
    case kEnemy_05:
        if (num == 1) {
            pEnemy = new Enemy05(type, num, x, y, angle, vitality, score, on_bg, item);
            sprite_frame_name = std::string(IMAGE_ENEMY_05_1);
        } else {
            pEnemy = new Enemy05(type, num, x, y, angle, vitality, score, on_bg, item);
            sprite_frame_name = std::string(IMAGE_ENEMY_05_2);
        }
        break;
    case kEnemy_06:
        pEnemy = new Enemy06(type, num, x, y, angle, vitality, score, on_bg, item);
        sprite_frame_name = std::string(IMAGE_ENEMY_06);
        break;
    case kEnemy_07:
        pEnemy = new Enemy07(type, num, x, y, angle, vitality, score, on_bg, item);
        sprite_frame_name = std::string(IMAGE_ENEMY_07);
       break;
    case kEnemy_08:
        pEnemy = new Enemy08(type, num, x, y, angle, vitality, score, on_bg, item);
        sprite_frame_name = std::string(IMAGE_ENEMY_08);
        break;
    case kEnemy_09:
        pEnemy = new Enemy09(type, num, x, y, angle, vitality, score, on_bg, item);
        sprite_frame_name = std::string(IMAGE_ENEMY_09);
        break;
    case kEnemyGround_01:
        pEnemy = new EnemyGround01(type, num, x, y, angle, vitality, score, on_bg, item);
        sprite_frame_name = std::string(IMAGE_ENEMY_GROUND_01);
        break;
    case kEnemyBoss_01:
        pEnemy = new EnemyBoss01(type, num, x, y, angle, vitality, score, on_bg, item);
        sprite_frame_name = std::string(IMAGE_ENEMY_BOSS_01);
        break;
    case kEnemyItem_01:
        pEnemy = new EnemyItem01(type, num, x, y, angle, vitality, score, on_bg, item);
        pEnemy->m_is_item = true;
        sprite_frame_name = std::string(IMAGE_ENEMY_ITEM_01);
        break;
    default:
        break;
    }

    if (pEnemy && pEnemy->initWithSpriteFrameName(sprite_frame_name.c_str())) {
        pEnemy->autorelease();
        pEnemy->scheduleUpdate();

        // Create shadow
        if (!pEnemy->m_on_background) {
            auto game_layer = GameLayer::getGameLayer();
            pEnemy->m_shadow = Sprite::createWithSpriteFrameName(sprite_frame_name.c_str());
            pEnemy->m_shadow_offset = Point(pEnemy->getContentSize().width * 0.13, pEnemy->getContentSize().height * -0.13);
            pEnemy->m_shadow->setPosition(pEnemy->getPosition() + pEnemy->m_shadow_offset);
            pEnemy->m_shadow->setColor(Color3B::BLACK);
            pEnemy->m_shadow->setScale(0.9f);
            auto enemy_shadow_layer = (Layer*)game_layer->getChildByTag(GameLayer::kTagEnemyShadowLayer);
            enemy_shadow_layer->addChild(pEnemy->m_shadow, GameLayer::kZOrderEnemyShadow, GameLayer::kTagEnemyShadow);
        }

        return pEnemy;
    }
    CC_SAFE_DELETE(pEnemy);
    return NULL;
}

void Enemy::update(float dt)
{
    if (m_finished) {
        return;
    }

    update2();

    setDxDyByMoveCommand();

    const Point& pos = this->getPosition() + Point(m_dx, m_dy);

    if (pos.y > m_winSize.height + getContentSize().height * 3 ||
        pos.y < CONTROL_AREA_HIGHT - getContentSize().height * 3 ||
        pos.x > m_winSize.width + getContentSize().width * 3 ||
        pos.x < -getContentSize().width * 3) {
        m_finished = true;
    } else {
        this->setPosition(pos);
        if (m_shadow) {
            m_shadow->setPosition(this->getPosition() + m_shadow_offset);
            m_shadow->setRotation(this->getRotation());
        }
        if (!m_on_background && m_laser_locked && m_laser_lock_mark) {
            m_laser_lock_mark->setPosition(this->getPosition());
        }
    }

    createBulletsByCommand();

    m_count++;
}

bool Enemy::damage(int point)
{
    if (m_finished) {
        return m_finished;
    }

    auto pAnimationCache = AnimationCache::getInstance();
    Animation* animation = NULL;
    switch (m_enemy_type) {
    case kEnemy_01: animation = pAnimationCache->getAnimation(ANIME_DAMAGE_ENEMY_01);
        break;
    case kEnemy_02: animation = pAnimationCache->getAnimation(ANIME_DAMAGE_ENEMY_02);
        break;
    case kEnemy_03: animation = pAnimationCache->getAnimation(ANIME_DAMAGE_ENEMY_03);
        break;
    case kEnemy_04: animation = pAnimationCache->getAnimation(ANIME_DAMAGE_ENEMY_04);
        break;
    case kEnemy_05:
        if (m_num == 1) {
            animation = pAnimationCache->getAnimation(ANIME_DAMAGE_ENEMY_05_1);
        } else {
            animation = pAnimationCache->getAnimation(ANIME_DAMAGE_ENEMY_05_2);
        }
        break;
    case kEnemy_06: animation = pAnimationCache->getAnimation(ANIME_DAMAGE_ENEMY_06);
        break;
    case kEnemy_07: animation = pAnimationCache->getAnimation(ANIME_DAMAGE_ENEMY_07);
        break;
    case kEnemy_08: animation = pAnimationCache->getAnimation(ANIME_DAMAGE_ENEMY_08);
        break;
    case kEnemy_09: animation = pAnimationCache->getAnimation(ANIME_DAMAGE_ENEMY_09);
        break;
    case kEnemyGround_01: animation = pAnimationCache->getAnimation(ANIME_DAMAGE_ENEMY_GROUND_01);
        break;
    case kEnemyBoss_01: animation = pAnimationCache->getAnimation(ANIME_DAMAGE_ENEMY_BOSS_01);
        break;
    case kEnemyItem_01:
        break;
    default:
        break;
    }
    if (animation) {
        this->runAction(CCAnimate::create(animation));
    }

    m_vitality -= point;

    if (m_enemy_type == kEnemyBoss_01) {
        auto game_layer = GameLayer::getGameLayer();
        const float rest = (m_vitality > 0) ? ((float)m_vitality / ENEMY_BOSS_01_VITALITY) : 0.0f;
        if (game_layer->getBoss01LifeGage()) {
            auto scale_to = ScaleTo::create(0.1f, rest, 1.0f);
            game_layer->getBoss01LifeGage()->runAction(scale_to);
        }
    }

    if (m_vitality <= 0) {
        emitItem();
        m_finished = true;
    }

    return m_finished;
}

void Enemy::emitItem()
{
    if (m_item != kItem_NULL) {
        auto game_layer = GameLayer::getGameLayer();

        if (game_layer->getMyShipBullet() + 1 < kMyBullet_Num) {
            const Point& pos = this->getPosition();
            auto item = Enemy::createEnemy(kEnemyItem_01, 1, pos.x, pos.y, 90, 1, 0, false, kItem_NULL);
            game_layer->addChild(item, GameLayer::kZOrderEnemy, GameLayer::kTagEnemy);
            std::list<Enemy*>& enemy_list = game_layer->getEnemyList();
            enemy_list.push_back(item);
        }
    }
}

void Enemy::createBulletsByCommand()
{
    for (auto itr = m_create_bullets_com_list.begin(); itr != m_create_bullets_com_list.end(); ) {
        if (itr->time_to_start == 0) {
            if (itr->count_of_shot > 0) {
                createStraightLineBullets(*itr);
                itr->angle += itr->angle_rate;
                itr->count_of_shot--;
                itr->time_to_start = itr->time_to_next_shot;
            }
            if (itr->count_of_shot == 0) {
                itr = m_create_bullets_com_list.erase(itr);
                continue;
            }
        } else {
            itr->time_to_start--;
        }
        ++itr;
    }
}

void Enemy::createStraightLineBullets(const CreateBulletCommand& com)
{
    if (com.bullets_count == 0) {
        return;
    }

    const Point& point = this->getPosition();
    const float x = (com.x == -1) ? point.x : com.x;
    const float y = (com.y == -1) ? point.y : com.y;

    auto game_layer = GameLayer::getGameLayer();
    int ang_1 = 0;
    if (com.bullets_count % 2) {
        ang_1 = com.angle - (com.distance_between_bullets * (int)(com.bullets_count / 2));
    } else {
        ang_1 = com.angle - (com.distance_between_bullets * (int)(com.bullets_count / 2 - 1)) - (int)(com.distance_between_bullets / 2);
    }
    while (ang_1 < 0) {
        ang_1 += 360;
    }

    for (int i = 0; i < com.bullets_count; ++i) {
        auto enemy_bullet = EnemyBullet::spriteWithFile(com.bullet_type, x, y, ang_1, com.speed, com.accel, com.speed_end,
            com.stop_time, com.move_time, com.stop_time_2, com.angle_2, com.speed_2, com.accel_2, com.speed_end_2, com.vanish_time);
        game_layer->addChild(enemy_bullet, GameLayer::kZOrderEnemyBullet, GameLayer::kTagEnemyBullet);
        ang_1 += com.distance_between_bullets;
    }
}

void Enemy::FinishSelf()
{
    m_finished = true;
}

Sprite* Enemy::createLifeGage()
{
    auto game_layer = GameLayer::getGameLayer();

    auto life_gage = Sprite::createWithSpriteFrameName(IMAGE_BOSS_LIFE_GAGE);
    life_gage->setAnchorPoint(Point(0, 0));
    life_gage->setPosition(Point(12, m_winSize.height * 0.90f));
    game_layer->addChild(life_gage, GameLayer::kZOrderLifeGage, GameLayer::kTagLifeGage);

    auto life_gage_bg = Sprite::createWithSpriteFrameName(IMAGE_BOSS_LIFE_GAGE_BG);
    life_gage_bg->setAnchorPoint(Point(0, 0));
    life_gage_bg->setPosition(Point(12, m_winSize.height * 0.90f));
    game_layer->addChild(life_gage_bg, GameLayer::kZOrderLifeGageBg, GameLayer::kTagLifeGage);

    return life_gage;
}

void Enemy::setDxDyByMoveCommand()
{
    int time = 0;
    if (m_move_com_list.size() > 0) {
        m_dx = 0;
        m_dy = 0;
    }
    for (auto elem : m_move_com_list) {
        if (m_count < time + elem.time_count) {
            if (elem.speed) {
                if (m_count == time) {
                    m_angle = elem.angle;
                    m_angle_f = elem.angle;
                    m_speed = elem.speed;
                }
                if (elem.sine_curve) {
                    m_dx = m_speed * MathUtil::cos(m_angle) + MathUtil::sin((m_count * (int)elem.angle_speed) % 360) * elem.x;
                    m_dy = m_speed * MathUtil::sin(m_angle) + MathUtil::sin((m_count * (int)elem.angle_speed) % 360) * elem.y;
                } else {
                    if (m_count > time) {
                        m_angle_f += elem.angle_speed;
                        m_angle = (int)m_angle_f;
                    }
                    m_dx = m_speed * MathUtil::cos(m_angle);
                    m_dy = m_speed * MathUtil::sin(m_angle);
                }
            } else {
                if (m_count == time) {
                    if (elem.x != 0 || elem.y != 0) {
                        int ang = CC_RADIANS_TO_DEGREES(atan2f(elem.y, elem.x));
                        if (ang < 0) {
                            ang += 360;
                        }
                        m_angle = ang;
                    }
                    m_speed = 0;
                }
                if (elem.smooth) {
                    const int t = m_count - time;
                    const int ang1 = 270 + 180 * t / elem.time_count;
                    const int ang2 = 270 + 180 * (t + 1) / elem.time_count;
                    m_dx = (MathUtil::sin(ang2) - MathUtil::sin(ang1)) * elem.x / 2;
                    m_dy = (MathUtil::sin(ang2) - MathUtil::sin(ang1)) * elem.y / 2;
                } else {
                    m_dx = elem.x / elem.time_count;
                    m_dy = elem.y / elem.time_count;
                }
            }
            break;
        }
        time += elem.time_count;
    }
}

void Enemy::moveBy(float x, float y, int time_count, bool smooth)
{
    MoveCommand move_command;
    move_command.time_count = time_count;
    move_command.x = x;
    move_command.y = y;
    move_command.angle = 0;
    move_command.speed = 0;
    move_command.angle_speed = 0.0f;
    move_command.smooth = smooth;
    move_command.sine_curve = false;
    m_move_com_list.push_back(move_command);
}

void Enemy::moveTo(float x, float y, int time_count, bool smooth)
{
    const Point& pos = this->getPosition();
    moveBy(x - pos.x, y - pos.y, time_count, smooth);
}

void Enemy::stopMove(int time_count)
{
    moveBy(0, 0, time_count, false);
}

void Enemy::moveWithAngleSpeed(int angle, float speed, float angle_speed, int time_count)
{
    MoveCommand move_command;
    move_command.time_count = time_count;
    move_command.x = 0;
    move_command.y = 0;
    move_command.angle = angle;
    move_command.speed = speed;
    move_command.angle_speed = angle_speed;
    move_command.smooth = false;
    move_command.sine_curve = false;
    m_move_com_list.push_back(move_command);
}

void Enemy::moveSineCurve(int angle, float speed, float width_x, float width_y, float angle_speed, int time_count)
{
    MoveCommand move_command;
    move_command.time_count = time_count;
    move_command.x = width_x;
    move_command.y = width_y;
    move_command.angle = angle;
    move_command.speed = speed;
    move_command.angle_speed = angle_speed;
    move_command.smooth = false;
    move_command.sine_curve = true;
    m_move_com_list.push_back(move_command);
}

void Enemy01::update2()
{
    //// Move
    if (m_count == 0) {
        moveWithAngleSpeed(m_angle, 4, 0.0f, 60);
        moveBy(0, -1, 40, false);
        moveBy(0, 100, 200, true);

        const Size win_size = Director::getInstance()->getWinSize();
        const Point& pos = this->getPosition();
        if (pos.x < win_size.width / 2) {
            moveWithAngleSpeed(m_angle - 60, 3, 0.0f, 1000);
        } else {
            moveWithAngleSpeed(m_angle + 60, 3, 0.0f, 1000);
        }
    }

    if (m_count < 90 || m_count > 300) {
        this->setRotation(360.0f - m_angle + 90.0f);
    }

    //// Bullets
    if (m_count > 0 && m_count % 100 == 0) {
        auto game_layer = GameLayer::getGameLayer();
        const Point& pos = this->getPosition();
        const short ang = game_layer->getAngleToMyShip(pos.x, pos.y);
        CreateBulletCommand create_bullet = { kEnemyBullet_01, -1, -1, ang, 3.0f, 0.0f, 0.0f, 3, 20, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 1, 0, 0, 0 };
        m_create_bullets_com_list.push_back(create_bullet);
    }
}

void Enemy02::update2()
{
    //// Move
    if (m_count == 0) {
        const Size win_size = Director::getInstance()->getWinSize();
        const Point& pos = this->getPosition();
        float speed = 2.0;
        if (pos.x < win_size.width / 2) {
            moveWithAngleSpeed(270, speed, 1.0f, 90);
            for (int i = 0; i < 10; i++) {
                moveWithAngleSpeed(360, speed, -1.0f, 180);
                moveWithAngleSpeed(180, speed, 1.0f, 180);
            }
        } else {
            moveWithAngleSpeed(270, speed, -1.0f, 90);
            for (int i = 0; i < 10; i++) {
                moveWithAngleSpeed(180, speed, 1.0f, 180);
                moveWithAngleSpeed(360, speed, -1.0f, 180);
            }
        }
    }

    if (m_angle < 330 && m_angle > 210) {
        this->setRotation(360.0f - m_angle + 90.0f);
    }

    //// Bullets
    if (m_count > 0 && m_count % 100 == 0) {
        auto game_layer = GameLayer::getGameLayer();
        const Point& pos = this->getPosition();
        const short ang = game_layer->getAngleToMyShip(pos.x, pos.y);
        CreateBulletCommand create_bullet = { kEnemyBullet_01, -1, -1, ang, 3.0f, 0.0f, 0.0f, 1, 0, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 1, 0, 0, 0 };
        m_create_bullets_com_list.push_back(create_bullet);
    }
}

void Enemy03::update2()
{
    //// Move
    if (m_count== 0) {
        const Size win_size = Director::getInstance()->getWinSize();
        const Point& pos = this->getPosition();
        const float speed = 1.5f;
        if (pos.x < win_size.width / 2) {
            moveWithAngleSpeed(360, speed, 1.0f, 90);
            for (int i = 0; i < 10; i++) {
                moveWithAngleSpeed(450, speed, -1.0f, 180);
                moveWithAngleSpeed(270, speed, 1.0f, 180);
            }
        } else {
            moveWithAngleSpeed(180, speed, -1.0f, 90);
            for (int i = 0; i < 10; i++) {
                moveWithAngleSpeed(90, speed, 1.0f, 180);
                moveWithAngleSpeed(270, speed, -1.0f, 180);
            }
        }
    }

    if ((m_angle > 300 && m_angle < 420) || (m_angle > 120 &&  m_angle < 240)) {
        this->setRotation(360.0f - m_angle + 90.0f);
    }

    //// Bullets
    if (m_count > 0 && m_count % 100 == 0) {
        auto game_layer = GameLayer::getGameLayer();
        const Point& pos = this->getPosition();
        const short ang = game_layer->getAngleToMyShip(pos.x, pos.y);
        CreateBulletCommand create_bullet = { kEnemyBullet_01, -1, -1, ang, 3.0f, 0.0f, 0.0f, 1, 0, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 1, 0, 0, 0 };
        m_create_bullets_com_list.push_back(create_bullet);
    }
}

void Enemy04::update2()
{
    //// Move
    if (m_count == 0) {
        moveBy(0, -200, 60, true);
        stopMove(60);
        for (int i = 0; i < 3; i++) {
            moveBy(-60, 0, 50, true);
            moveBy(0, 60, 50, true);
            moveBy(60, 0, 50, true);
            moveBy(0, -60, 50, true);
            stopMove(80);
        }
        stopMove(40);
        moveWithAngleSpeed(270, 4, 0.0f, 1000);
    }

    this->setRotation(360.0f - m_angle + 90.0f);

    //// Bullets
    if ((m_count > 0 && m_count == 100) || m_count == 200 || m_count == 500 || m_count == 800 || m_count == 1100) {
        auto game_layer = GameLayer::getGameLayer();
        const Point& pos = this->getPosition();
        const short ang = game_layer->getAngleToMyShip(pos.x, pos.y);
        CreateBulletCommand create_bullet = { kEnemyBullet_01, -1, -1, ang, 3.0f, 0.0f, 0.0f, 1, 0, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 1, 0, 0, 0 };
        m_create_bullets_com_list.push_back(create_bullet);
    }
    if (m_count == 320 || m_count == 600 || m_count == 880) {
        auto game_layer = GameLayer::getGameLayer();
        const Point& pos = this->getPosition();
        const short ang = game_layer->getAngleToMyShip(pos.x, pos.y);
        CreateBulletCommand create_bullet = { kEnemyBullet_05, -1, -1, ang, 3.0f, 0.0f, 0.0f, 3, 30, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 15, 0, 5, 0 };
        m_create_bullets_com_list.push_back(create_bullet);
    }
}

void Enemy05::update2()
{
    //// Move
    if (m_count == 0) {
        moveBy(0, -200, 80, true);
        stopMove(60);
        moveBy(0, 120, 120, true);
        stopMove(60);
        for (int i = 0; i < 2; i++) {
            moveBy(-80, 0, 120, true);
            stopMove(10);
            moveBy(80, 0, 120, true);
            stopMove(10);
            moveBy(80, 0, 120, true);
            stopMove(10);
            moveBy(-80, 0, 120, true);
            stopMove(10);
        }
        stopMove(100);
        moveWithAngleSpeed(270, 4, 0.0f, 1000);
    }

    //// Bullets
    if (m_num == 1) {
        if (m_count > 150 && m_count % 3 == 0) {
            static short ang = 270;
            CreateBulletCommand create_bullet = { kEnemyBullet_01, -1, -1, ang, 2.0f, 0.0f, 0.0f, 1, 0, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 1, 0, 0, 0 };
            m_create_bullets_com_list.push_back(create_bullet);
            ang = (ang + rand()) % 360;
            if (ang < 180) ang += 180;
        }
        if (m_count >= 700 && m_count <= 1400 && m_count % 100 == 0) {
            CreateBulletCommand create_bullet = { kEnemyBullet_04, -1, -1, 270, 1.5f, 0.0f, 0.0f, 12, 30, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 4, 0, 20, 15 };
            m_create_bullets_com_list.push_back(create_bullet);
        }
    } else {
        if (m_count > 150 && m_count <= 1200 && m_count % 200 == 0) {
            auto game_layer = GameLayer::getGameLayer();
            const Point& pos = this->getPosition();
            const short ang = game_layer->getAngleToMyShip(pos.x, pos.y);
            CreateBulletCommand create_bullet = { kEnemyBullet_06, -1, -1, ang, 3.0f, 0.0f, 0.0f, 3, 30, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 15, 0, 5, 0 };
            m_create_bullets_com_list.push_back(create_bullet);
        }
    }
}

void Enemy06::update2()
{
    //// Move
    if (m_count == 0) {
        const Size win_size = Director::getInstance()->getWinSize();
        const Point& pos = this->getPosition();
        const float speed = 1.0f;
        int angle = 0;
        if (pos.x < win_size.width * 0.2f) {
            angle = 0;
        } else if (pos.x > win_size.width * 0.8f) {
            angle = 180;
        } else {
            angle = 270;
        }
        moveWithAngleSpeed(angle, speed, 0.0f, 80);
        moveWithAngleSpeed(angle, speed, 1.0f, 270 - (((m_num - 1) / 3) * 90));
        stopMove(100);
        moveWithAngleSpeed(angle + 270 - (((m_num - 1) / 3) * 90), speed, 1.0f, 180);
        stopMove(100);
        moveWithAngleSpeed(angle + 450 - (((m_num - 1) / 3) * 90), speed, 1.0f, 180);
        stopMove(150);
        moveWithAngleSpeed(0, 3, 0.0f, 1000);
    }

    this->setRotation(360.0f - m_angle + 90.0f);

    //// Bullets
    const int count_1 = 80 + 270 - (((m_num - 1) / 3) * 90) + 30;
    if (m_count == count_1 || m_count == count_1 + 280 || m_count == count_1 + 560) {
        CreateBulletCommand create_bullet = { kEnemyBullet_05, -1, -1, (short)(180 - ((m_num - 1) / 3) * 20), 3.0f, 0.0f, 0.0f, 4, 90, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 15, 0, 5, 0 };
        m_create_bullets_com_list.push_back(create_bullet);
    }
    if (m_count == count_1 + 60 || m_count == count_1 + 340) {
        auto game_layer = GameLayer::getGameLayer();
        const Point& pos = this->getPosition();
        const short ang = game_layer->getAngleToMyShip(pos.x, pos.y);
        CreateBulletCommand create_bullet2 = { kEnemyBullet_04, -1, -1, ang, 1.8f, 0.0f, 0.0f, 4, 30, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 2, 0, 10, 15 };
        m_create_bullets_com_list.push_back(create_bullet2);
    }
    if (m_count == count_1 + 620) {
        auto game_layer = GameLayer::getGameLayer();
        const Point& pos = this->getPosition();
        const short ang = game_layer->getAngleToMyShip(pos.x, pos.y);
        CreateBulletCommand create_bullet2 = { kEnemyBullet_04, -1, -1, ang, 1.8f, 0.0f, 0.0f, 4, 30, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 4, 0, 10, 15 };
        m_create_bullets_com_list.push_back(create_bullet2);
    }
}

void Enemy07::update2()
{
    //// Move
    if (m_count == 0) {
        m_speed = 2.0f;
    }
    if (m_count > 1200) {
        m_angle = 270;
        m_dx = m_speed * MathUtil::cos(m_angle);
        m_dy = m_speed * MathUtil::sin(m_angle);
        this->setRotation(360.0f - m_angle + 90.0f);
    } else if (m_count % 30 == 0) {
        auto game_layer = GameLayer::getGameLayer();
        const Point& pos = this->getPosition();
        const int ang = game_layer->getAngleToMyShip(pos.x, pos.y);
        if (m_angle >= 0 && m_angle < 90) {
            if (ang >= 0 && ang < 90) {
                m_angle = ang;
            } else if (ang >= 90 && ang < 180) {
                m_angle += 30;
            } else if (ang >= 180 && ang < 270) {
                m_angle += 30;
            } else {
                m_angle += 340;
            }
        } else if (m_angle >= 90 && m_angle < 180) {
            if (ang >= 0 && ang < 90) {
                m_angle -= 30;
            } else if (ang >= 90 && ang < 180) {
                m_angle = ang;
            } else if (ang >= 180 && ang < 270) {
                m_angle += 30;
            } else {
                m_angle += 30;
            }
        } else if (m_angle >= 180 && m_angle < 270) {
            if (ang >= 0 && ang < 90) {
                m_angle -= 30;
            } else if (ang >= 90 && ang < 180) {
                m_angle -= 30;
            } else if (ang >= 180 && ang < 270) {
                m_angle = ang;
            } else {
                m_angle += 30;
            }
        } else {
            if (ang >= 0 && ang < 90) {
                m_angle += 30;
            } else if (ang >= 90 && ang < 180) {
                m_angle -= 30;
            } else if (ang >= 180 && ang < 270) {
                m_angle -= 30;
            } else {
                m_angle = ang;
            }
        }
        m_dx = m_speed * MathUtil::cos(m_angle);
        m_dy = m_speed * MathUtil::sin(m_angle);
        this->setRotation(360.0f - m_angle + 90.0f);
    }
}

void Enemy08::update2()
{
    //// Move
    if (m_count == 0) {
        moveBy(0, -150, 100, true);
        moveBy(0, 50, 50, true);
        for (int i = 0; i < 6; i++) {
            moveBy(-20, 0, 100, true);
            moveBy(20, 0, 100, true);
        }
        moveWithAngleSpeed(270, 4, 0.0f, 1000);
    }

    //// Bullets
    if (m_count > 0 && m_count % 100 == 0) {
        CreateBulletCommand create_bullet = { kEnemyBullet_03, -1, -1, 270, 1.5f, 0.0f, 0.0f, 12, 30, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 3, 0, 10, 15 };
        m_create_bullets_com_list.push_back(create_bullet);
    }
    if (m_count > 400 && m_count % 100 > 40 && m_count % 100 < 70 && m_count % 3 == 0) {
        CreateBulletCommand create_bullet_1 = { kEnemyBullet_04, -1, -1, (short)(180 + rand() % 60), 2.0f, 0.0f, 0.0f, 1, 0, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 1, 0, 0, 0 };
        CreateBulletCommand create_bullet_2 = { kEnemyBullet_04, -1, -1, (short)(240 + rand() % 60), 2.0f, 0.0f, 0.0f, 1, 0, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 1, 0, 0, 0 };
        CreateBulletCommand create_bullet_3 = { kEnemyBullet_04, -1, -1, (short)(300 + rand() % 60), 2.0f, 0.0f, 0.0f, 1, 0, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 1, 0, 0, 0 };
        m_create_bullets_com_list.push_back(create_bullet_1);
        m_create_bullets_com_list.push_back(create_bullet_2);
        m_create_bullets_com_list.push_back(create_bullet_3);
    }
}

void EnemyGround01::update2()
{
    //// Bullets
    if (m_count > 100 && m_count % 100 == 0) {
        auto game_layer = GameLayer::getGameLayer();
        const Point& pos = this->getPosition();
        const short ang = game_layer->getAngleToMyShip(pos.x, pos.y);
        CreateBulletCommand create_bullet = { kEnemyBullet_02, -1, -1, ang, 3.0f, 0.0f, 0.0f, 1, 0, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 3, 0, 15, 0 };
        m_create_bullets_com_list.push_back(create_bullet);
    }
}

void EnemyBoss01::update2()
{
    //// Move
    if (m_count > 100) {
        m_dx = MathUtil::sin(m_count % 360) * 0.7f;
    } else if (m_count == 0) {
        auto game_layer = GameLayer::getGameLayer();
        auto life_gage = createLifeGage();
        game_layer->setBoss01LifeGage(life_gage);

        stopMove(30);
        moveBy(0, -140, 70, true);
    } else if (m_count == 100) {
        m_move_com_list.clear();
        m_dy = 0;
    }

    //// Bullets
    if (m_count > 100) {
        auto game_layer = GameLayer::getGameLayer();
        const Point& pos = this->getPosition();
        const int count_2 = (m_count - 101) % 8600;
        const int n_lap = (m_count - 101) / 8600;

        int appear_count_min = 0;
        int appear_count_max = 0;


        //// bullets pattern: 18way
        appear_count_min = appear_count_max;
        appear_count_max = appear_count_min + 650;
        if (count_2 >= appear_count_min && count_2 < appear_count_max) {
            if ((count_2 - appear_count_min) % 80 == 0) {
                const short angle_rate = 1 + rand() % 10;
                CreateBulletCommand create_bullet = { kEnemyBullet_01, -1, -1, 270, 2.5f, 0.0f, 0.0f, 18, 20, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 5, 0, 15, angle_rate };
                m_create_bullets_com_list.push_back(create_bullet);
            }

            if (n_lap > 0) {
                // 5way
                if (count_2 % 200 == 0) {
                    const short ang = game_layer->getAngleToMyShip(pos.x, pos.y);
                    CreateBulletCommand create_bullet = { kEnemyBullet_06, -1, -1, ang, 3.5f, 0.0f, 0.0f, 5, 20, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 15, 0, 5, 0 };
                    m_create_bullets_com_list.push_back(create_bullet);
                }
            }
        }
        appear_count_max += 100;


        //// bullets pattern: らせん
        appear_count_min = appear_count_max;
        appear_count_max = appear_count_min + 1080;
        if (count_2 >= appear_count_min && count_2 < appear_count_max) {
            if ((count_2 - appear_count_min) % 360 == 0) {
                const short ang_rate = ((count_2 - appear_count_min) % 720 == 0) ? -12 : 12;

                CreateBulletCommand create_bullet1 = { kEnemyBullet_01, -1, -1, 90 + 360, 5.0f, -0.05f, 2.5f, 1, 0, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 90, 0, 4, ang_rate };
                m_create_bullets_com_list.push_back(create_bullet1);

                CreateBulletCommand create_bullet2 = { kEnemyBullet_01, -1, -1, 180 + 360, 5.0f, -0.05f, 2.5f, 1, 0, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 90, 0, 4, ang_rate }; 
                m_create_bullets_com_list.push_back(create_bullet2);

                CreateBulletCommand create_bullet3 = { kEnemyBullet_01, -1, -1, 270 + 360, 5.0f, -0.05f, 2.5f, 1, 0, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 90, 0, 4, ang_rate };
                m_create_bullets_com_list.push_back(create_bullet3);

                CreateBulletCommand create_bullet4 = { kEnemyBullet_01, -1, -1, 360 + 360, 5.0f, -0.05f, 2.5f, 1, 0, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 90, 0, 4, ang_rate };
                m_create_bullets_com_list.push_back(create_bullet4);
            }

            if (n_lap > 0) {
                // 5way
                if (count_2 % 200 == 0) {
                    const short ang = game_layer->getAngleToMyShip(pos.x, pos.y);
                    CreateBulletCommand create_bullet = { kEnemyBullet_06, -1, -1, ang, 3.5f, 0.0f, 0.0f, 5, 20, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 15, 0, 5, 0 };
                    m_create_bullets_com_list.push_back(create_bullet);
                }
            }
        }
        appear_count_max += 60;


        //// bullets pattern: ランダム
        appear_count_min = appear_count_max;
        appear_count_max = appear_count_min + 800;
        if (count_2 >= appear_count_min && count_2 < appear_count_max) {
            const short ang = 180 + (m_count % 4) * 45 + rand() % 45;
            CreateBulletCommand create_bullet = { kEnemyBullet_01, -1, -1, ang, 2.8f, 0.0f, 0.0f, 1, 0, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 1, 0, 0, 0 };
            m_create_bullets_com_list.push_back(create_bullet);

            if (n_lap > 0) {
                // 5way
                if (count_2 % 200 == 0) {
                    const short ang = game_layer->getAngleToMyShip(pos.x, pos.y);
                    CreateBulletCommand create_bullet = { kEnemyBullet_06, -1, -1, ang, 3.5f, 0.0f, 0.0f, 5, 20, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 15, 0, 5, 0 };
                    m_create_bullets_com_list.push_back(create_bullet);
                }
            }
        }
        appear_count_max += 10;


        //// bullets pattern: 棒
        appear_count_min = appear_count_max;
        appear_count_max = appear_count_min + 1000;
        if (count_2 >= appear_count_min && count_2 < appear_count_max) {
            if (count_2 == appear_count_min) {
                auto enemy1 = Enemy::createEnemy(kEnemy_09, 6, pos.x, pos.y, 0, ENEMY_09_VITALITY, ENEMY_09_SCORE, false, kItem_NULL);
                auto enemy2 = Enemy::createEnemy(kEnemy_09, 7, pos.x, pos.y, 0, ENEMY_09_VITALITY, ENEMY_09_SCORE, false, kItem_NULL);
                game_layer->addChild(enemy1, GameLayer::kZOrderEnemy, GameLayer::kTagEnemy);
                game_layer->addChild(enemy2, GameLayer::kZOrderEnemy, GameLayer::kTagEnemy);
                std::list<Enemy*>& enemy_list = game_layer->getEnemyList();
                enemy_list.push_back(enemy1);
                enemy_list.push_back(enemy2);
            }

            if (n_lap > 0) {
                if (count_2 % 60 == 0) {
                    CreateBulletCommand create_bullet = { kEnemyBullet_04, -1, -1, 270, 2.5f, 0.0f, 0.0f, 6, 30, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 1, 0, 0, 0 };
                    m_create_bullets_com_list.push_back(create_bullet);
                }
            }
        }
        appear_count_max += 10;


        //// bullets pattern: 翼
        appear_count_min = appear_count_max;
        appear_count_max = appear_count_min + 950;
        if (count_2 >= appear_count_min && count_2 < appear_count_max) {
            if (count_2 == appear_count_min) {
                auto enemy1 = Enemy::createEnemy(kEnemy_09, 8, pos.x, pos.y, 0, ENEMY_09_VITALITY, ENEMY_09_SCORE, false, kItem_NULL);
                auto enemy2 = Enemy::createEnemy(kEnemy_09, 9, pos.x, pos.y, 0, ENEMY_09_VITALITY, ENEMY_09_SCORE, false, kItem_NULL);
                game_layer->addChild(enemy1, GameLayer::kZOrderEnemy, GameLayer::kTagEnemy);
                game_layer->addChild(enemy2, GameLayer::kZOrderEnemy, GameLayer::kTagEnemy);
                std::list<Enemy*>& enemy_list = game_layer->getEnemyList();
                enemy_list.push_back(enemy1);
                enemy_list.push_back(enemy2);
            }

            if (n_lap > 0) {
                if (count_2 % 60 == 0) {
                    CreateBulletCommand create_bullet = { kEnemyBullet_04, -1, -1, 270, 2.5f, 0.0f, 0.0f, 6, 30, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 1, 0, 0, 0 };
                    m_create_bullets_com_list.push_back(create_bullet);
                }
            }
        }
        appear_count_max += 10;


        //// bullets pattern: 回転円
        appear_count_min = appear_count_max;
        appear_count_max = appear_count_min + 1100;
        if (count_2 >= appear_count_min && count_2 < appear_count_max) {
            if (count_2 == appear_count_min) {
                auto enemy1 = Enemy::createEnemy(kEnemy_09, 10, pos.x, pos.y, 0, ENEMY_09_VITALITY, ENEMY_09_SCORE, false, kItem_NULL);
                auto enemy2 = Enemy::createEnemy(kEnemy_09, 11, pos.x, pos.y, 0, ENEMY_09_VITALITY, ENEMY_09_SCORE, false, kItem_NULL);
                game_layer->addChild(enemy1, GameLayer::kZOrderEnemy, GameLayer::kTagEnemy);
                game_layer->addChild(enemy2, GameLayer::kZOrderEnemy, GameLayer::kTagEnemy);
                std::list<Enemy*>& enemy_list = game_layer->getEnemyList();
                enemy_list.push_back(enemy1);
                enemy_list.push_back(enemy2);
            }

            if (n_lap > 0) {
                if (count_2 % 60 == 0) {
                    CreateBulletCommand create_bullet = { kEnemyBullet_04, -1, -1, 270, 2.5f, 0.0f, 0.0f, 6, 30, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 1, 0, 0, 0 };
                    m_create_bullets_com_list.push_back(create_bullet);
                }
            }
        }
        appear_count_max += 10;


        //// bullets pattern: 上下移動砲台
        appear_count_min = appear_count_max;
        appear_count_max = appear_count_min + 1300;
        if (count_2 >= appear_count_min && count_2 < appear_count_max) {
            if (count_2 == appear_count_min) {
                auto enemy1 = Enemy::createEnemy(kEnemy_09, 1, pos.x, pos.y, 0, ENEMY_09_VITALITY, ENEMY_09_SCORE, false, kItem_NULL);
                game_layer->addChild(enemy1, GameLayer::kZOrderEnemy, GameLayer::kTagEnemy);
                std::list<Enemy*>& enemy_list = game_layer->getEnemyList();
                enemy_list.push_back(enemy1);
            }
            if (count_2 == appear_count_min + 250) {
                auto enemy1 = Enemy::createEnemy(kEnemy_09, 2, pos.x, pos.y, 0, ENEMY_09_VITALITY, ENEMY_09_SCORE, false, kItem_NULL);
                game_layer->addChild(enemy1, GameLayer::kZOrderEnemy, GameLayer::kTagEnemy);
                std::list<Enemy*>& enemy_list = game_layer->getEnemyList();
                enemy_list.push_back(enemy1);
            }

            if (n_lap > 0) {
                if (count_2 % 60 == 0) {
                    CreateBulletCommand create_bullet = { kEnemyBullet_04, -1, -1, 270, 2.5f, 0.0f, 0.0f, 6, 30, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 1, 0, 0, 0 };
                    m_create_bullets_com_list.push_back(create_bullet);
                }
            }
        }
        appear_count_max += 100;


        //// bullets pattern: 後方3way
        appear_count_min = appear_count_max;
        appear_count_max = appear_count_min + 1050;
        if (count_2 >= appear_count_min && count_2 < appear_count_max) {
            if ((count_2 - appear_count_min) % 80 == 0) {
                const int n = (count_2 - appear_count_min < 320) ? 1 : 2;
                for (int i = 0; i < n; ++i) {
                    auto enemy1 = Enemy::createEnemy(kEnemy_09, 5, pos.x, pos.y, 0, ENEMY_09_VITALITY, ENEMY_09_SCORE, false, kItem_NULL);
                    game_layer->addChild(enemy1, GameLayer::kZOrderEnemy, GameLayer::kTagEnemy);
                    std::list<Enemy*>& enemy_list = game_layer->getEnemyList();
                    enemy_list.push_back(enemy1);
                }
            }

            if (n_lap > 0) {
                if (count_2 % 60 == 0) {
                    CreateBulletCommand create_bullet = { kEnemyBullet_04, -1, -1, 270, 2.5f, 0.0f, 0.0f, 6, 30, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 1, 0, 0, 0 };
                    m_create_bullets_com_list.push_back(create_bullet);
                }
            }
        }
        appear_count_max += 320;
    }
}

void Enemy09::update2()
{
    switch (m_num) {
    case 1:
    case 2:
        //// Move
        if (m_count == 0) {
            const Point& pos = this->getPosition();
            if ( m_num == 1) {
                moveBy(m_winSize.width - pos.x - this->getContentSize().width * 0.5, -345, 50, true);
            } else {
                moveBy(-pos.x + this->getContentSize().width * 0.5, -345, 50, true);
            }
            stopMove(50);
            moveBy(0,  395, 150, true);
            for (int i = 0; i < 2; ++i) {
                stopMove(50);
                moveBy(0, -395, 150, true);
                stopMove(50);
                moveBy(0,  395, 150, true);
            }
            if (m_num == 1) {
                moveWithAngleSpeed(0, 5.0f, 0.0f, 1000);
                moveBy(100, 0, 10, true);
            } else {
                moveWithAngleSpeed(180, 5.0f, 0.0f, 1000);
            }
        }
        //// Bullets: 上下移動砲台
        if (m_count % 10 == 0) {
            if ((m_count > 100 && m_count < 250) || (m_count > 500 && m_count < 650) || (m_count > 900 && m_count < 1050)) {
                const short ang = (m_num == 1) ? 225 : 315;
                CreateBulletCommand create_bullet = { kEnemyBullet_02, -1, -1, ang, 2.0f, 0.0f, 0.0f, 6, 15, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 1, 0, 0, 0 };
                m_create_bullets_com_list.push_back(create_bullet);
            }
        }
        break;
    case 3:
        //// Move
        if (m_count == 0) {
            moveTo(160, 300, 50, true);
            stopMove(600);
            moveWithAngleSpeed(180, 5.0f, 0.0f, 1000);
        }
        //// Bullets: 星型
        if ((m_count - 50) == 0) {
            CreateBulletCommand create_bullet = { kEnemyBullet_01, -1, -1, 270, 2.0f, 0.0f, 0.0f, 6, 60, 10, 50, 30, -1, 0.0f, 0.002f, 1.5f, 0, 1, 0, 0, 0 };
            const Point& pos = this->getPosition();
            const float x0[] = { 0, 0.59f, -0.95f, 0.95f, -0.59f, 0 };
            const float y0[] = { -1, 0.81f, -0.31f, -0.31f, 0.81f, -1 };
            const int star_size = 50;
            const int bullet_count_on_line = 10;
            for (int i = 0; i < 5; ++i) {
                for (int j = 0; j < bullet_count_on_line; ++j) {
                    const float x1 = x0[i] + (x0[i + 1] - x0[i]) * j / bullet_count_on_line;
                    const float y1 = y0[i] + (y0[i + 1] - y0[i]) * j / bullet_count_on_line;
                    create_bullet.x = pos.x + x1 * star_size;
                    create_bullet.y = pos.y + y1 * star_size - 10;
                    create_bullet.angle_2 = rand() % 360;
                    m_create_bullets_com_list.push_back(create_bullet);
                }
            }
        }
        break;
    case 4:
        //// Move
        if (m_count == 0) {
            moveTo(160, 300, 50, true);
            stopMove(600);
            moveWithAngleSpeed(180, 5.0f, 0.0f, 1000);
        }
        //// Bullets: 円型
        if ((m_count - 50) == 0) {
            CreateBulletCommand create_bullet = { kEnemyBullet_01, -1, -1, 270, 2.0f, 0.0f, 0.0f, 6, 60, 10, 50, 30, -1, 0.0f, 0.002f, 1.5f, 0, 1, 0, 0, 0 };
            const Point& pos = this->getPosition();
            const float r = 50.0f;
            for (int i = 0; i < 40; ++i) {
                create_bullet.x = pos.x + MathUtil::cos(i * 9) * r;
                create_bullet.y = pos.y + MathUtil::sin(i * 9) * r;
                create_bullet.angle_2 = rand() % 360;
                m_create_bullets_com_list.push_back(create_bullet);
            }
        }
        break;
    case 5:
        //// Move
        if (m_count == 0) {
            int s = (rand() % 2 == 0) ? 1 : -1;
            int angle_dx = (rand() % 30) * s;
            moveWithAngleSpeed(270 + angle_dx, 2.5f, (float)-s * 0.5f, 1000);
        }
        //// Bullets: 後方3way
        if (m_count > 10) {
            if (m_count % 20 == 0) {
                CreateBulletCommand create_bullet = { kEnemyBullet_02, -1, -1, (short)(m_angle + 180), 1.0f, 0.0f, 0.0f, 3, 30, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 1, 0, 0, 0 };
                m_create_bullets_com_list.push_back(create_bullet);
            }
        }
        break;
    case 6:
    case 7:
        //// Move
        if (m_count == 0) {
            if (m_num == 6) {
                moveTo(70, 320, 50, true);
               stopMove(900);
                moveWithAngleSpeed(180, 5.0f, 0.0f, 1000);
            } else {
                moveTo(250, 320, 50, true);
                stopMove(900);
                moveWithAngleSpeed(0, 5.0f, 0.0f, 1000);
            }
        }
        //// Bullets: 棒
        if (m_count > 50) {
            if ((m_count - 50) % 50 == 0) {
                const short ang = (m_num == 6) ? 90 + rand() % 30 : 90 - rand() % 30;
                for (int i = 0; i < 5; ++i) {
                    CreateBulletCommand create_bullet_1 = { kEnemyBullet_02, -1, -1, ang, 4.0f - i * 0.8f, 0.0f, 0.0f, 1, 0, 0, 25, 25, (short)(ang + 170), 2.5f + (4 - i) * 0.3f, 0.0f, 0.0f, 0, 1, 0, 0, 0 };
                    CreateBulletCommand create_bullet_2 = { kEnemyBullet_02, -1, -1, ang, 4.0f - i * 0.8f, 0.0f, 0.0f, 1, 0, 0, 25, 25, (short)(ang + 190), 2.5f + (4 - i) * 0.3f, 0.0f, 0.0f, 0, 1, 0, 0, 0 };
                    m_create_bullets_com_list.push_back(create_bullet_1);
                    m_create_bullets_com_list.push_back(create_bullet_2);
                }
            }
            if (m_count % 30 == 0) {
                auto game_layer = GameLayer::getGameLayer();
                const Point& pos = this->getPosition();
                const short ang = game_layer->getAngleToMyShip(pos.x, pos.y);
                const short ang1 = (m_num == 6) ? std::min((int)ang, 270) : std::max((int)ang, 270);
                const short ang2 = (m_num == 6) ? 360 : 180;
                const float speed = (ang1 == 270) ? 2.0f : 4.0f;
                CreateBulletCommand create_bullet1 = { kEnemyBullet_01, -1, -1, ang1, speed, 0.0f, 0.0f, 1, 0, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 1, 0, 0, 0 };
                CreateBulletCommand create_bullet2 = { kEnemyBullet_01, -1, -1, (short)(ang2 + 10), speed, 0.0f, 0.0f, 1, 0, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 1, 0, 0, 0 };
                CreateBulletCommand create_bullet3 = { kEnemyBullet_01, -1, -1, (short)(ang2 - 10), speed, 0.0f, 0.0f, 1, 0, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 1, 0, 0, 0 };
                m_create_bullets_com_list.push_back(create_bullet1);
                if (m_count % 60 == 0) {
                    m_create_bullets_com_list.push_back(create_bullet2);
                } else {
                    m_create_bullets_com_list.push_back(create_bullet3);
                }
            }
        }
        break;
    case 8:
    case 9:
        //// Move
        if (m_count == 0) {
            if (m_num == 8) {
                moveTo(80, 320, 50, true);
               stopMove(900);
                moveWithAngleSpeed(180, 5.0f, 0.0f, 1000);
            } else {
                moveTo(240, 320, 50, true);
                stopMove(900);  
                moveWithAngleSpeed(0, 5.0f, 0.0f, 1000);
            }
        }
        //// Bullets: 翼
        if (m_count > 50) {
            if ((m_count - 50) % 50 == 0) {
                auto game_layer = GameLayer::getGameLayer();
                const Point& pos = this->getPosition();
                const short ang = game_layer->getAngleToMyShip(pos.x, pos.y) - 6 * 3;
                for (int i = 0; i < 7; ++i) {
                    float dy = (i == 3) ? 12.0f : (i == 2 || i == 4) ? 8.0f : (i == 1 || i == 5) ? 4.0f : 0.0f;
                    CreateBulletCommand create_bullet1 = { kEnemyBullet_01, -1, pos.y - dy, (short)(ang + i * 6), 2.0f, 0.0f, 0.0f, 1, 0, 0, -1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 1, 0, 0, 0 };
                    m_create_bullets_com_list.push_back(create_bullet1);
                }
            }
        }
        break;
    case 10:
    case 11:
        //// Move
        if (m_count == 0) {
            if (m_num == 10) {
                moveTo(40, 320, 50, true);
                for (int i = 0; i < 5; ++i) {
                    moveBy(0, -50, 100, true);
                    moveBy(0, 50, 100, true);
                }
                moveWithAngleSpeed(180, 5.0f, 0.0f, 1000);
            } else {
                moveTo(280, 270, 50, true);
                for (int i = 0; i < 5; ++i) {
                    moveBy(0, 50, 100, true);
                    moveBy(0, -50, 100, true);
                }
                moveWithAngleSpeed(0, 5.0f, 0.0f, 1000);
            }
        }
        //// Bullets: 回転円
        if (m_count > 50 && m_count <= 1000) {
            const int t1 = 0;
            if ((m_count - 50) % 50 == t1) {
                auto game_layer = GameLayer::getGameLayer();
                const Point& pos = this->getPosition();
                const short ang = game_layer->getAngleToMyShip(pos.x, pos.y);
                const float orig_speed = 3.0f;
                CreateBulletCommand create_bullet = { kEnemyBullet_02, -1, -1, ang, orig_speed, 0.0f, 0.0f, 1, 0, 0, 0, 0, -1, 0.0f, 0.0f, 0.0f, 0, 1, 0, 0, 0 };

                const float r = 15.0f + rand() % 35;
                for (int i = 0; i < 10; ++i) {
                    create_bullet.x = pos.x + MathUtil::cos(i * 36) * r;
                    create_bullet.y = pos.y + MathUtil::sin(i * 36) * r;
                    create_bullet.speed -= MathUtil::cos((ang + 360) - i * 36) * 1.0f;
                    m_create_bullets_com_list.push_back(create_bullet);
                    create_bullet.speed = orig_speed;
                }
            }
        }
    default:
        break;
    }
}

void EnemyItem01::update2()
{
    if (m_count == 0) {
        stopMove(50);
        moveWithAngleSpeed(270, 2.0f, 0, 1000);
    }
}
