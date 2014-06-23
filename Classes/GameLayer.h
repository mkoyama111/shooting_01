#ifndef GAME_LAYER_H__
#define GAME_LAYER_H__

#include <vector>
#include <memory>
#include "cocos2d.h"
#include "EnemyController.h"

class MyShip;
class Enemy;
class EnemyController;

static const float CONTROL_AREA_HIGHT = 480 * 0.1f;

class GameLayer : public cocos2d::Layer
{
public:
    enum kTag {
        kTagBackgroundLayer,
        kTagMyBullet,
        kTagEnemyBullet,
        kTagMyShip,
        kTagEnemy,
        kTagEnemyShadow,
        kTagEnemyBoss,
        kTagEnemyShadowLayer,
        kTagParticleLayer,
        kTagLaserLayer,
        kTagLaserLockLayer,
        kTagLabelLayer,
        kTagControlLayer,
        kTagGameInfoLabel,
        kTagHiScoreLabel,
        kTagScoreLabel,
        kTagLifeGage
    };

    enum kZOrder {
        kZOrderBackground = 1,
        kZOrderEnemyOnBackground,
        kZOrderEnemyShadow,
        kZOrderMyShip,
        kZOrderEnemy,
        kZOrderMyLaserLock,
        kZOrderMyBullet,
        kZOrderEnemyBullet,
        kZOrderMyLaserStreak,
        kZOrderParticleLayer,
        kZOrderControlLayer,
        kZOrderGameInfoLabel,
        kZOrderLifeGageBg,
        kZOrderLifeGage,
    };

    virtual bool init();
    static cocos2d::Scene* scene();
    CREATE_FUNC(GameLayer);

    GameLayer();
    virtual ~GameLayer();

    CC_SYNTHESIZE(cocos2d::Sprite*, m_boss_01_life_gage, Boss01LifeGage);

    virtual bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
    virtual void onTouchMoved(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
    virtual void onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);

    void update(float dt);

    static GameLayer* getGameLayer();

    static int getBackGroundMusic();

    std::list<Enemy*>& getEnemyList();

    void addParticle(cocos2d::ParticleSystemQuad* particle);

    void createExplosion(kTag tag, cocos2d::Point point, bool play_se);

    int getAngleToMyShip(float x, float y) const;
    const cocos2d::Point& getMyShipPos() const;

    void getMoveBackgroundParam(float& duration, cocos2d::Point& delta_pos);

    int plusMyShipLife();
    int minusMyShipLife();

protected:

    void createAnimationCache();
    void createDamageAnimationCache(cocos2d::AnimationCache* pAnimationCache,
        const char* img_1, const char* img_2, const char* name);
    void createBackground();
    void createParticleLayer();
    void createEnemyShadowLayer();
    void createLaserLayer();
    void createLabelLayer();
    void createControlLayer();
    void createGameScoreLabel();
    void createLifeGage();
    void createMyShip();

    void cleanupEnemies();
    void checkCollision();
    void updateLabel();
    void resetGame();
    void getItem(kEnemy enemy_type, bool play_se);
    void powerupWeapon();
    void resetWeapon();

    cocos2d::Rect shrinkRect(cocos2d::Rect rect, float scale);

    void gameOver();
    void gameClear();
    void backToTitle(cocos2d::Ref* pSender);
    void moveToEnding(float dt);

    void saveHighScore();
    void loadHighScore(int& high_score);

    cocos2d::Size m_winSize;

    std::list<Enemy*> m_enemy_list;
    std::vector<cocos2d::Node*> m_removed_node_vec;

    MyShip* m_myShip;
    cocos2d::Layer* m_particle_layer;
    cocos2d::Layer* m_label_layer;
    cocos2d::Point m_move_delta_pos;

    std::vector<cocos2d::SpriteBatchNode*> m_my_bullet_batchNode;
    std::vector<cocos2d::SpriteBatchNode*> m_my_laser_batchNode;
    std::vector<cocos2d::SpriteBatchNode*> m_enemy_bullet_batchNode;
    std::vector<cocos2d::SpriteBatchNode*> m_enemy_batchNode;
    std::vector<cocos2d::SpriteBatchNode*> m_my_laser_lock_batchNode;

    std::unique_ptr<EnemyController> m_enemy_controller;

    std::vector<cocos2d::Sprite*> m_my_ship_life_gage;

    bool m_game_over;
    bool m_game_clear;
    int m_count;
    int m_hi_score;
    int m_score;
    int m_my_ship_life_count;
    float m_move_duration;
    float m_since_game_over;
};

#endif
