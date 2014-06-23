#include <cmath>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include "SimpleAudioEngine.h"
#include "TitleLayer.h"
#include "EndingLayer.h"
#include "GameConfig.h"
#include "GameLayer.h"
#include "MyShip.h"
#include "MyBullet.h"
#include "Enemy.h"
#include "EnemyBullet.h"
#include "EnemyController.h"
#include "MathUtil.h"
#include "GameData.h"

USING_NS_CC;
using namespace CocosDenshion;

static const float DURATION_AFTER_GAME_OVER = 20.0f;

static GameLayer* s_game_layer;
static int s_back_ground_music = BACK_GROUND_MUSIC;

GameLayer::GameLayer() : m_boss_01_life_gage(NULL), m_myShip(NULL), m_particle_layer(NULL), m_label_layer(NULL),
    m_move_delta_pos(Point::ZERO), m_enemy_controller(new EnemyController), m_game_over(false), m_game_clear(false),
    m_count(0), m_hi_score(0), m_score(0), m_my_ship_life_count(MY_SHIP_LIFE_COUNT), m_move_duration(0),
    m_since_game_over(0)
{
    srand((unsigned)time(NULL));
}

GameLayer::~GameLayer()
{
    s_game_layer = NULL;
}

Scene* GameLayer::scene()
{
    Scene* scene = Scene::create();
    GameLayer* layer = GameLayer::create();
    scene->addChild(layer);
    s_game_layer = layer;
    return scene;
}

bool GameLayer::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    auto touch_listener = EventListenerTouchOneByOne::create();
    touch_listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
    touch_listener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved, this);
    touch_listener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch_listener, this);    

    MathUtil::Init();

    m_winSize = Director::getInstance()->getWinSize();

    if (GameLayer::getBackGroundMusic()) {
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0f);
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(BACK_GROUND_MUSIC_01, false);
    }

    SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);
    SimpleAudioEngine::getInstance()->preloadEffect(SE_ENEMY_EXPLOSION);
    SimpleAudioEngine::getInstance()->preloadEffect(SE_ENEMY_BOSS_EXPLOSION);
    SimpleAudioEngine::getInstance()->preloadEffect(SE_MY_SHIP_EXPLOSION);
    SimpleAudioEngine::getInstance()->preloadEffect(SE_MY_LASER_LOCK);
    SimpleAudioEngine::getInstance()->preloadEffect(SE_GET_ITEM_01);

    SpriteFrameCache* sprite_frame_cache = SpriteFrameCache::getInstance();
    sprite_frame_cache->addSpriteFramesWithFile("shooting_01.plist");
    SpriteBatchNode* batch_node = SpriteBatchNode::create("shooting_01.png");
    this->addChild(batch_node);

    createAnimationCache();
    createBackground();
    createParticleLayer();
    createEnemyShadowLayer();
    createLaserLayer();
    createLabelLayer();
    createControlLayer();
    createMyShip();

    scheduleUpdate();

    return true;
}

GameLayer* GameLayer::getGameLayer()
{
    return s_game_layer;
}

int GameLayer::getBackGroundMusic()
{
    return s_back_ground_music;
}

std::list<Enemy*>& GameLayer::getEnemyList()
{
    return m_enemy_list;
}

void GameLayer::addParticle(cocos2d::ParticleSystemQuad* particle)
{
    m_particle_layer->addChild(particle);
}

int GameLayer::getAngleToMyShip(float x, float y) const
{
    if (m_myShip) {
       const Point pos = m_myShip->getPosition();
       int ang = CC_RADIANS_TO_DEGREES(atan2f((pos.y - y), (pos.x - x)));
       if (ang < 0) {
           ang += 360;
       }
       return ang;
    }
    return 0;
}

const cocos2d::Point& GameLayer::getMyShipPos() const
{
    return m_myShip->getPosition();
}

void GameLayer::getMoveBackgroundParam(float& duration, cocos2d::Point& delta_pos)
{
    duration = m_move_duration;
    delta_pos = m_move_delta_pos;
    return;
}

int GameLayer::plusMyShipLife()
{
    if (m_my_ship_life_count < (int)m_my_ship_life_gage.size()) {
        m_my_ship_life_gage[m_my_ship_life_count]->setColor(Color3B(0x62, 0xfc, 0x24));
        m_my_ship_life_count++;
    }
    return m_my_ship_life_count;
}

int GameLayer::minusMyShipLife()
{
    if (m_my_ship_life_count > 0) {
        m_my_ship_life_gage[m_my_ship_life_count - 1]->setColor(Color3B(0x0, 0x0, 0x0));
        m_my_ship_life_count--;
    }
    return m_my_ship_life_count;
}

void GameLayer::createAnimationCache()
{
    AnimationCache::getInstance()->destroyInstance();
    AnimationCache* pAnimationCache = AnimationCache::getInstance();

    createDamageAnimationCache(pAnimationCache, IMAGE_DAMAGE_ENEMY_01, IMAGE_ENEMY_01, ANIME_DAMAGE_ENEMY_01);
    createDamageAnimationCache(pAnimationCache, IMAGE_DAMAGE_ENEMY_02, IMAGE_ENEMY_02, ANIME_DAMAGE_ENEMY_02);
    createDamageAnimationCache(pAnimationCache, IMAGE_DAMAGE_ENEMY_03, IMAGE_ENEMY_03, ANIME_DAMAGE_ENEMY_03);
    createDamageAnimationCache(pAnimationCache, IMAGE_DAMAGE_ENEMY_04, IMAGE_ENEMY_04, ANIME_DAMAGE_ENEMY_04);
    createDamageAnimationCache(pAnimationCache, IMAGE_DAMAGE_ENEMY_05_1, IMAGE_ENEMY_05_1, ANIME_DAMAGE_ENEMY_05_1);
    createDamageAnimationCache(pAnimationCache, IMAGE_DAMAGE_ENEMY_05_2, IMAGE_ENEMY_05_2, ANIME_DAMAGE_ENEMY_05_2);
    createDamageAnimationCache(pAnimationCache, IMAGE_DAMAGE_ENEMY_06, IMAGE_ENEMY_06, ANIME_DAMAGE_ENEMY_06);
    createDamageAnimationCache(pAnimationCache, IMAGE_DAMAGE_ENEMY_07, IMAGE_ENEMY_07, ANIME_DAMAGE_ENEMY_07);
    createDamageAnimationCache(pAnimationCache, IMAGE_DAMAGE_ENEMY_08, IMAGE_ENEMY_08, ANIME_DAMAGE_ENEMY_08);
    createDamageAnimationCache(pAnimationCache, IMAGE_DAMAGE_ENEMY_09, IMAGE_ENEMY_09, ANIME_DAMAGE_ENEMY_09);
    createDamageAnimationCache(pAnimationCache, IMAGE_DAMAGE_ENEMY_GROUND_01, IMAGE_ENEMY_GROUND_01, ANIME_DAMAGE_ENEMY_GROUND_01);
    createDamageAnimationCache(pAnimationCache, IMAGE_DAMAGE_ENEMY_BOSS_01, IMAGE_ENEMY_BOSS_01, ANIME_DAMAGE_ENEMY_BOSS_01);
}

void GameLayer::createDamageAnimationCache(cocos2d::AnimationCache* pAnimationCache,
        const char* img_1, const char* img_2, const char* name)
{
    Animation* animation = Animation::create();
    animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(img_1));
    animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(img_2));
    animation->setDelayPerUnit(0.4f);
    animation->setLoops(1);
    pAnimationCache->addAnimation(animation, name);
}

void GameLayer::createBackground()
{
    Layer* background_layer = Layer::create();
    SpriteBatchNode* batchNode = SpriteBatchNode::create(IMAGE_BACKGROUND);
    background_layer->addChild(batchNode, kZOrderBackground);
    int height = 0;

    for (int i = 0; i < 24; i++) {
        Sprite* bg = Sprite::createWithTexture(batchNode->getTexture());
        bg->setAnchorPoint(Point(0, 0));
        bg->setPosition(Point(0, height));
        background_layer->addChild(bg, kZOrderBackground);
        height += bg->getContentSize().height;
    }
    this->addChild(background_layer, kZOrderBackground, kTagBackgroundLayer);

    m_move_duration = (float)8200 / 60;
    m_move_delta_pos = Point(0, -(height - m_winSize.height));
    MoveBy* move = MoveBy::create(m_move_duration, m_move_delta_pos);
    background_layer->runAction(move);
}

void GameLayer::createParticleLayer()
{
    m_particle_layer = Layer::create();
    m_particle_layer->setContentSize(m_winSize);
    m_particle_layer->setPosition(Point(0, 0));
    this->addChild(m_particle_layer, kZOrderParticleLayer, kTagParticleLayer);
}

void GameLayer::createEnemyShadowLayer()
{
    Layer* enemy_shadow_layer = Layer::create();
    enemy_shadow_layer->setContentSize(m_winSize);
    enemy_shadow_layer->setPosition(Point(0, 0));
    this->addChild(enemy_shadow_layer, kZOrderEnemyShadow, kTagEnemyShadowLayer);
}

void GameLayer::createLaserLayer()
{
    Layer* laser_layer = Layer::create();
    laser_layer->setContentSize(m_winSize);
    laser_layer->setPosition(Point(0, 0));
    this->addChild(laser_layer, kZOrderMyLaserStreak, kTagLaserLayer);

    Layer* laser_lock_layer = Layer::create();
    laser_lock_layer->setContentSize(m_winSize);
    laser_lock_layer->setPosition(Point(0, 0));
    this->addChild(laser_lock_layer, kZOrderMyLaserLock, kTagLaserLockLayer);
}


void GameLayer::createLabelLayer()
{
    m_label_layer = Layer::create();
    m_label_layer->setContentSize(m_winSize);
    m_label_layer->setPosition(Point(0, 0));
    this->addChild(m_label_layer, kZOrderGameInfoLabel, kTagLabelLayer);
}

void GameLayer::createControlLayer()
{
    LayerColor* layer = LayerColor::create(Color4B(140, 140, 140, 255), m_winSize.width, CONTROL_AREA_HIGHT);
    layer->setPosition(Point(0, 0));
    this->addChild(layer, kZOrderControlLayer, kTagControlLayer);

    createGameScoreLabel();
    createLifeGage();
}

void GameLayer::createGameScoreLabel()
{
    loadHighScore(m_hi_score);

    TTFConfig ttf_config(TTF_FONT_01, 18);

    auto lbl_hi_score = Label::createWithTTF(ttf_config, "Hi-Score:");
    lbl_hi_score->setPosition(Point(235, m_winSize.height * 0.95f));
    lbl_hi_score->setAnchorPoint(Point(1, 0));
    this->addChild(lbl_hi_score, kZOrderGameInfoLabel, kTagGameInfoLabel);

    auto hi_score = Label::createWithTTF(ttf_config, "0");
    hi_score->setPosition(Point(315, m_winSize.height * 0.95f));
    hi_score->setAnchorPoint(Point(1, 0));
    hi_score->setString(CCString::createWithFormat("%d", m_hi_score)->getCString());
    this->addChild(hi_score, kZOrderGameInfoLabel, kTagHiScoreLabel);

    auto lbl_score = Label::createWithTTF(ttf_config, "Score:");
    lbl_score->setPosition(Point(55, m_winSize.height * 0.95f));
    lbl_score->setAnchorPoint(Point(1, 0));
    this->addChild(lbl_score, kZOrderGameInfoLabel, kTagGameInfoLabel);

    auto score = Label::createWithTTF(ttf_config, "0");
    score->setPosition(Point(125, m_winSize.height * 0.95f));
    score->setAnchorPoint(Point(1, 0));
    this->addChild(score, kZOrderGameInfoLabel, kTagScoreLabel);
}

void GameLayer::createLifeGage()
{
    TTFConfig ttf_config(TTF_FONT_01, 18);
    auto lbl_life = Label::createWithTTF(ttf_config, "Life:");
    lbl_life->setPosition(Point(10, CONTROL_AREA_HIGHT * 0.60f));
    lbl_life->setAnchorPoint(Point(0, 0));
    this->addChild(lbl_life, kZOrderGameInfoLabel, kTagGameInfoLabel);

    Sprite* life_gage_bg = Sprite::createWithSpriteFrameName(IMAGE_MY_SHIP_LIFE_GAGE_BG);
    life_gage_bg->setAnchorPoint(Point(0, 0));
    life_gage_bg->setPosition(Point(10, CONTROL_AREA_HIGHT * 0.15f));
    this->addChild(life_gage_bg, kZOrderLifeGageBg, kTagLifeGage);

    for (int i = 0; i < 10; ++i) {
        int xpos = 20 + i * 28;
        Sprite* life_gage = Sprite::createWithSpriteFrameName(IMAGE_MY_SHIP_LIFE_GAGE);
        life_gage->setAnchorPoint(Point(0, 0));
        life_gage->setPosition(Point(xpos, CONTROL_AREA_HIGHT * 0.22f));
        if (i < m_my_ship_life_count) {
            life_gage->setColor(Color3B(0x62, 0xfc, 0x24));
        } else {
            life_gage->setColor(Color3B(0, 0, 0));
        }
        this->addChild(life_gage, kZOrderLifeGage, kTagLifeGage);

        m_my_ship_life_gage.push_back(life_gage);
    }
}

void GameLayer::createMyShip()
{
    m_myShip = MyShip::spriteWithFile(IMAGE_MY_SHIP_01, kMyBullet_01, MY_BULLET_01_DAMAGE, MY_SHIP_VITALITY);
    m_myShip->setPosition(Point(m_winSize.width * 0.5f, CONTROL_AREA_HIGHT + m_myShip->getContentSize().height * 0.5f + 5));
    this->addChild(m_myShip, kZOrderMyShip, kTagMyShip);
}

bool GameLayer::onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent)
{
    m_myShip->setIsTouched(true);
    m_myShip->setTouchPos(pTouch->getLocation());
    return true;
}

void GameLayer::onTouchMoved(cocos2d::Touch* pTouch, cocos2d::Event* pEvent)
{
    m_myShip->setIsTouched(true);
    m_myShip->setTouchPos(pTouch->getLocation());
}

void GameLayer::onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent)
{
    m_myShip->setIsTouched(false);
}

Rect GameLayer::shrinkRect(cocos2d::Rect rect, float scale)
{
    if (scale < 1.0) {
        return Rect(rect.origin.x + rect.size.width * (1.0f - scale) * 0.5,
            rect.origin.y + rect.size.height * (1.0f - scale) * 0.5,
            rect.size.width * scale,
            rect.size.height * scale);
    } else {
        return rect;
    }
}

void GameLayer::update(float dt)
{
    cleanupEnemies();

    if (m_enemy_controller->getGameStage() == kGameStage_End) {
        gameClear();
    }

    if (m_my_ship_life_count == 0) {
        if (!m_game_over) {
            gameOver();
        }
        m_since_game_over += dt;
        if (m_since_game_over > DURATION_AFTER_GAME_OVER) {
            backToTitle(NULL);
        }
    } else {

        m_enemy_controller->createEnemies(m_enemy_list);
    }

    if (!m_game_over && !m_game_clear) {
        checkCollision();
    }

    updateLabel();

    m_count++;
}

void GameLayer::cleanupEnemies()
{
    for (auto itr = m_enemy_list.begin(); itr != m_enemy_list.end(); ) {
        if ((*itr)->getFinished()) {
            if ((*itr)->getLaserLocked()) {
                (*itr)->setVisible(false);
                if ((*itr)->getShadow()) {
                    (*itr)->getShadow()->setVisible(false);
                }
            } else {
                Enemy* enemy_tmp = *itr;
                itr = m_enemy_list.erase(itr);
                enemy_tmp->removeFromParentAndCleanup(true);
                continue;
            }
        }
        itr++;
    }
}

void GameLayer::checkCollision()
{
    const Rect myShipRect = shrinkRect(m_myShip->getBoundingBox(), 0.5f); // 自機の当たり判定を小さめにとる
    const int my_bullet_damage = m_myShip->getBulletDamage();

    auto node_vec = this->getChildren(); 
    for (auto obj: node_vec) {

        if (!obj) {
            break;
        }

        Node* pNode = static_cast<Node*>(obj);
        const int tag = pNode->getTag();

        if (tag == kTagEnemyBullet) { // 敵弾 → 自機の当たり判定

            if (myShipRect.containsPoint(pNode->getPosition()) && m_myShip->getSinceDamaged() == 0) {

                EnemyBullet* enemy_bullet = static_cast<EnemyBullet*>(pNode);

                if (!enemy_bullet->getPenetrate()) {
                    m_removed_node_vec.push_back(pNode);
                }

                if (m_myShip->damage(1)) {
                    createExplosion(kTagMyShip, m_myShip->getPosition(), true);
                }
            }
        } else if (tag == kTagMyBullet) { // 自弾 → 敵機の当たり判定

            for (auto elem : m_enemy_list) {

                if (elem->getFinished() || elem->getOnBackground() || elem->getIsItem()) {
                    continue;
                }

                if (elem->getBoundingBox().containsPoint(pNode->getPosition())) {

                    m_removed_node_vec.push_back(pNode);

                    if (elem->damage(my_bullet_damage)) {
                        if (!m_game_over) {
                            m_score += elem->getScore();
                        }
                        createExplosion((kTag)elem->getTag(), elem->getPosition(), true);
                    } else {
                        m_score += 10;
                    }
                }
            }
        } else if (tag == kTagEnemy || tag == kTagEnemyBoss) { // 敵機 → 自機の当たり判定

            Enemy* enemy = static_cast<Enemy*>(pNode);
            if (enemy->getFinished() || enemy->getOnBackground()) {
                continue;
            }

            if (myShipRect.intersectsRect(pNode->getBoundingBox())) {

                if (enemy->getIsItem()) {
                    enemy->damage(1);
                    getItem(enemy->getEnemyType(), true);
                    continue;
                }

                if (m_myShip->getSinceDamaged() != 0 || m_my_ship_life_count == 0) {
                    continue;
                }

                if (enemy->damage(MY_SHIP_CRASH_DAMAGE)) {
                    if (!m_game_over) {
                        m_score += enemy->getScore();
                    }
                    createExplosion((kTag)enemy->getTag(), enemy->getPosition(), true);
                }

                if (m_myShip->damage(1)) {
                    createExplosion(kTagMyShip, m_myShip->getPosition(), true);
                }
            }
        }
    }

    for (auto elem : m_removed_node_vec) {
        elem->removeFromParentAndCleanup(true);
    }
    m_removed_node_vec.clear();
}

void GameLayer::createExplosion(kTag tag, cocos2d::Point point, bool play_se)
{
    ParticleSystemQuad* explosion = NULL;
    if (tag == kTagEnemyBoss) {
        explosion = ParticleSystemQuad::create("explosion_boss_01.plist");
        explosion->setPosition(point - Point(0, 20));
    } else {
        explosion = ParticleSystemQuad::create("explosion_01.plist");
        explosion->setPosition(point);
    }
    explosion->setAutoRemoveOnFinish(true);
    m_particle_layer->addChild(explosion);

    if (tag == kTagMyShip) {
        if (play_se) {
            SimpleAudioEngine::getInstance()->playEffect(SE_MY_SHIP_EXPLOSION);
        }
        if (m_my_ship_life_count == 0) {
            ParticleFire* fire = ParticleFire::create();
            fire->setScale(0.2f);
            fire->setPosition(point);
            m_particle_layer->addChild(fire);
        } else {
            m_myShip->setPosition(Point(m_winSize.width * 0.5f, CONTROL_AREA_HIGHT + m_myShip->getContentSize().height * 0.5f + 5 - 60));
            resetWeapon();
        }
    } else if (tag == kTagEnemyBoss) {
        if (!m_game_over) {
            if (play_se) {
                SimpleAudioEngine::getInstance()->playEffect(SE_ENEMY_BOSS_EXPLOSION);
            }
            m_enemy_controller->setBossDestroyed(true);
        }
    } else {
        if (!m_game_over) {
            if (play_se) {
                SimpleAudioEngine::getInstance()->playEffect(SE_ENEMY_EXPLOSION);
            }
        }
    }
}

void GameLayer::getItem(kEnemy enemy_type, bool play_se)
{
    switch (enemy_type) {
    case  kEnemyItem_01:
        if (play_se) {
            SimpleAudioEngine::getInstance()->playEffect(SE_GET_ITEM_01);
        }
        powerupWeapon();
        break;
    default:
        break;
    }
}

void GameLayer::powerupWeapon()
{
    if (m_myShip->getBullet() + 1 < kMyBullet_Num) {
        m_myShip->setBullet((kMyBullet)(m_myShip->getBullet() + 1));
    }
}

void GameLayer::resetWeapon()
{
    m_myShip->setBullet(kMyBullet_01);
}

void GameLayer::updateLabel()
{
    if (m_score > m_hi_score) {
         Label* lbl = static_cast<Label*>(this->getChildByTag(kTagHiScoreLabel));
         lbl->setString(String::createWithFormat("%d", m_score)->getCString());
    }
    {
        std::string aaa("abc");
        Label* lbl = static_cast<Label*>(this->getChildByTag(kTagScoreLabel));
        lbl->setString(String::createWithFormat("%d", m_score)->getCString());
    }
}

void GameLayer::resetGame()
{
    Scene* scene = GameLayer::scene();
    Director::getInstance()->replaceScene(scene);
}

void GameLayer::gameOver()
{
    if (!m_game_over) {
        m_game_over = true;
        this->getEventDispatcher()->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);
        m_myShip->setIsTouched(false);

        const float delay_time = 1.0f;
        const float fade_in_time = 2.0f;

#ifdef WIN32
        // Visual Studio 2012 では #pragma execution_character_set("utf-8") 
        // がサポートされていないため、ワイド文字からUTF-8に変換して、
        // CCLabelBMFont::create()にUTF-8文字列を渡す。
        char buf[128];
        ::WideCharToMultiByte(CP_UTF8, 0, L"ゲームオーバー", -1, buf, sizeof(buf), NULL, NULL);
        auto game_over_label = Label::createWithBMFont(BM_FONT_01, buf);
#else
        auto game_over_label = Label::createWithBMFont(BM_FONT_01, "ゲームオーバー");
#endif
        game_over_label->setPosition(Point(m_winSize.width * 0.5f, m_winSize.height * 0.7f));
        game_over_label->setColor(Color3B(255, 255, 255));
        game_over_label->setOpacity(0);
        m_label_layer->addChild(game_over_label, kZOrderGameInfoLabel, kTagGameInfoLabel);
        game_over_label->runAction(CCSequence::create(
            CCDelayTime::create(delay_time),
            CCFadeIn::create(fade_in_time),
            NULL));

#ifdef WIN32
        ::WideCharToMultiByte(CP_UTF8, 0, L"タイトルにもどる", -1, buf, sizeof(buf), NULL, NULL);
        auto back_label = Label::createWithBMFont(BM_FONT_01, buf);
#else
        auto back_label = Label::createWithBMFont(BM_FONT_01, "タイトルにもどる");
#endif
        back_label->setColor(Color3B(255, 255, 255));
        back_label->setOpacity(0);
        MenuItemLabel* back_btn = MenuItemLabel::create(back_label, CC_CALLBACK_1(GameLayer::backToTitle, this));
        back_btn->setPosition(Point(m_winSize.width * 0.5f, m_winSize.height * 0.45f));
        Menu* menu = Menu::create(back_btn, NULL);
        menu->setPosition(Point::ZERO);
        m_label_layer->addChild(menu, kZOrderGameInfoLabel, kTagGameInfoLabel);
        back_label->runAction(CCSequence::create(
            CCDelayTime::create(delay_time),
            CCFadeIn::create(fade_in_time),
            NULL));

        if (GameLayer::getBackGroundMusic()) {
            CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        }

        saveHighScore();
    }
}

void GameLayer::gameClear()
{
    if (!m_game_clear) {
        m_game_clear = true;
        this->getEventDispatcher()->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);
        m_myShip->setIsTouched(false);

        if (GameLayer::getBackGroundMusic()) {
            CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        }

        this->scheduleOnce(schedule_selector(GameLayer::moveToEnding), 4.0f);

        saveHighScore();
        GameData::getInstance()->setScore(m_score);
    }
}

void GameLayer::backToTitle(cocos2d::Ref* pSender)
{
    Scene* scene = TitleLayer::scene();
    TransitionFade* transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}

void GameLayer::moveToEnding(float dt)
{
    Scene* scene = EndingLayer::scene();
    TransitionFade* transition = TransitionFade::create(6.0f, scene);
    Director::getInstance()->replaceScene(transition);
}

void GameLayer::saveHighScore()
{
    UserDefault* user_default = UserDefault::getInstance();
    const int old_high_score = user_default->getIntegerForKey("HighScore", 0);
    if (m_score > old_high_score) {
        user_default->setIntegerForKey("HighScore", m_score);
        user_default->flush();
    }
}

void GameLayer::loadHighScore(int& high_score)
{
    high_score = UserDefault::getInstance()->getIntegerForKey("HighScore", 0);
}
