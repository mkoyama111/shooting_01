#include "SimpleAudioEngine.h"
#include "GameConfig.h"
#include "GameLayer.h"
#include "Enemy.h"
#include "EnemyController.h"

USING_NS_CC;
using namespace CocosDenshion;

EnemyControllerState::EnemyControllerState(kGameStage game_stage) : m_game_stage(game_stage), m_state_count(0)
{
    m_winSize = Director::getInstance()->getWinSize();
}

#define DEF_ENEMY_CONTROLLER_STATE(X) \
    class X : public EnemyControllerState { \
    public: \
        X(kGameStage game_stage) : EnemyControllerState(game_stage) {} \
        virtual void createEnemy(const EnemyController* context, int count, std::list<Enemy*>& enemy_list); \
        virtual std::shared_ptr<EnemyControllerState> nextState(const EnemyController* context) const; \
    }

DEF_ENEMY_CONTROLLER_STATE(EnemyControllerState01);
DEF_ENEMY_CONTROLLER_STATE(EnemyControllerState02);

EnemyController::EnemyController() : m_state(std::make_shared<EnemyControllerState01>(kGameStage_01)), m_boss_destroyed(false), m_count(0)
{
}

kGameStage EnemyController::getGameStage() const
{
    const auto next_state = m_state->nextState(this);
    if (next_state) {
        return next_state->getGameStage();
    }
    return m_state->getGameStage();
}

void EnemyController::createEnemies(std::list<Enemy*>& enemy_list)
{
    const auto next_state = m_state->nextState(this);
    if (next_state) {
        m_boss_destroyed = false;
        m_state = next_state;
    }
    m_state->createEnemy(this, m_count, enemy_list);
    m_count++;
}

void EnemyControllerState::createEnemyByCommand(std::list<Enemy*>& enemy_list)
{
    auto game_layer = GameLayer::getGameLayer();

    for (auto itr = m_create_enemy_com_list.begin(); itr != m_create_enemy_com_list.end(); ) {
        if (itr->count_to_start == 0) {
            auto enemy1 = Enemy::createEnemy(itr->type, itr->num, itr->x, itr->y, itr->angle, itr->vitality, itr->score, itr->on_background, itr->item);
            if (itr->on_background) {
                game_layer->addChild(enemy1, GameLayer::kZOrderEnemyOnBackground, GameLayer::kTagEnemy);
            } else {
                if (itr->type >= kEnemyBoss_01) {
                    game_layer->addChild(enemy1, GameLayer::kZOrderEnemy, GameLayer::kTagEnemyBoss);
                } else if (itr->type >= kEnemy_05 && itr->num > 1) {
                    game_layer->addChild(enemy1, GameLayer::kZOrderEnemy2, GameLayer::kTagEnemy);
                } else {
                    game_layer->addChild(enemy1, GameLayer::kZOrderEnemy, GameLayer::kTagEnemy);
                }
            }
            enemy_list.push_back(enemy1);
            itr = m_create_enemy_com_list.erase(itr);
            continue;
        } else {
            itr->count_to_start--;
        }
        ++itr;
    }
}

void EnemyControllerState01::createEnemy(const EnemyController* context, int count, std::list<Enemy*>& enemy_list)
{
    const float width = m_winSize.width;
    const float height = m_winSize.height;

    if (m_state_count == 0) {
        const kEnemy enemy_type = kEnemyGround_01;
        const int vitality = ENEMY_GROUND_01_VITALITY;
        const int score = ENEMY_GROUND_01_SCORE;
        CreateEnemyCommand com_list[] = {
            {  450, enemy_type, 1, width * 0.18f, height, 0, vitality, score, true, kItem_NULL },
            {  460, enemy_type, 1, width * 0.88f, height, 0, vitality, score, true, kItem_NULL },
            {  700, enemy_type, 1, width * 0.18f, height, 0, vitality, score, true, kItem_NULL },
            {  750, enemy_type, 1, width * 0.93f, height, 0, vitality, score, true, kItem_NULL },
            {  800, enemy_type, 1, width * 0.28f, height, 0, vitality, score, true, kItem_NULL },
            {  850, enemy_type, 1, width * 0.88f, height, 0, vitality, score, true, kItem_NULL },
            { 1000, enemy_type, 1, width * 0.90f, height, 0, vitality, score, true, kItem_NULL },
            { 1050, enemy_type, 1, width * 0.10f, height, 0, vitality, score, true, kItem_NULL },
            { 1200, enemy_type, 1, width * 0.30f, height, 0, vitality, score, true, kItem_NULL },
            { 1250, enemy_type, 1, width * 0.95f, height, 0, vitality, score, true, kItem_NULL },
            { 1300, enemy_type, 1, width * 0.10f, height, 0, vitality, score, true, kItem_NULL },
            { 1350, enemy_type, 1, width * 0.90f, height, 0, vitality, score, true, kItem_NULL },
            { 1750, enemy_type, 1, width * 0.85f, height, 0, vitality, score, true, kItem_NULL },
            { 1750, enemy_type, 1, width * 0.15f, height, 0, vitality, score, true, kItem_NULL },
            { 2600, enemy_type, 1, width * 0.90f, height, 0, vitality, score, true, kItem_NULL },
            { 2600, enemy_type, 1, width * 0.20f, height, 0, vitality, score, true, kItem_NULL },
            { 2700, enemy_type, 1, width * 0.70f, height, 0, vitality, score, true, kItem_NULL },
            { 2750, enemy_type, 1, width * 0.10f, height, 0, vitality, score, true, kItem_NULL },
            { 2800, enemy_type, 1, width * 0.85f, height, 0, vitality, score, true, kItem_NULL },
            { 2850, enemy_type, 1, width * 0.35f, height, 0, vitality, score, true, kItem_NULL },
            { 4600, enemy_type, 1, width * 0.85f, height, 0, vitality, score, true, kItem_NULL },
            { 4650, enemy_type, 1, width * 0.10f, height, 0, vitality, score, true, kItem_NULL },
            { 4900, enemy_type, 1, width * 0.12f, height, 0, vitality, score, true, kItem_NULL },
            { 4950, enemy_type, 1, width * 0.93f, height, 0, vitality, score, true, kItem_NULL },
            { 5200, enemy_type, 1, width * 0.10f, height, 0, vitality, score, true, kItem_NULL },
            { 5250, enemy_type, 1, width * 0.91f, height, 0, vitality, score, true, kItem_NULL },
            { 7900, enemy_type, 1, width * 0.81f, height, 0, vitality, score, true, kItem_NULL },
            { 7950, enemy_type, 1, width * 0.21f, height, 0, vitality, score, true, kItem_NULL },
         };
        for (int i = 0; i < sizeof(com_list) / sizeof(com_list[0]); i++) {
            m_create_enemy_com_list.push_back(com_list[i]);
        }
    }

    if (m_state_count == 0) {
        {
            const kEnemy enemy_type = kEnemy_01;
            const int vitality = ENEMY_01_VITALITY;
            const int score = ENEMY_01_SCORE;
            CreateEnemyCommand com_list[] = {
                { 200, enemy_type, 1, width * 0.60f, height + 10, 270, vitality, score, false, kItem_NULL },
                { 230, enemy_type, 2, width * 0.75f, height + 60, 270, vitality, score, false, kItem_NULL },
                { 260, enemy_type, 3, width * 0.90f, height + 90, 270, vitality, score, false, kItem_NULL },
                { 380, enemy_type, 4, width * 0.40f, height + 10, 270, vitality, score, false, kItem_NULL },
                { 410, enemy_type, 5, width * 0.25f, height + 60, 270, vitality, score, false, kItem_NULL },
                { 440, enemy_type, 6, width * 0.10f, height + 90, 270, vitality, score, false, kItem_NULL },
            };
            for (int i = 0; i < sizeof(com_list) / sizeof(com_list[0]); i++) {
                m_create_enemy_com_list.push_back(com_list[i]);
            }
        }
        {
            const kEnemy enemy_type = kEnemy_08;
            const int vitality = ENEMY_08_VITALITY;
            const int score = ENEMY_08_SCORE;
            CreateEnemyCommand com_list[] = {
                { 550, enemy_type, 1, width * 0.6f, height + 10, 270, vitality, score, false, kItem_01 },
            };
            for (int i = 0; i < sizeof(com_list) / sizeof(com_list[0]); i++) {
                m_create_enemy_com_list.push_back(com_list[i]);
            }
        }
    }

    if (m_state_count == 1500) {
        const kEnemy enemy_type = kEnemy_02;
        const int vitality = ENEMY_02_VITALITY;
        const int score = ENEMY_02_SCORE;
        CreateEnemyCommand com_list[] = {
            {   0, enemy_type,  1,         5, height, 272, vitality, score, false, kItem_NULL },
            {  40, enemy_type,  2,         5, height, 272, vitality, score, false, kItem_NULL },
            {  80, enemy_type,  3,         5, height, 272, vitality, score, false, kItem_NULL },
            { 120, enemy_type,  4,         5, height, 272, vitality, score, false, kItem_NULL },
            { 160, enemy_type,  5,         5, height, 272, vitality, score, false, kItem_NULL },
            { 200, enemy_type,  6,         5, height, 272, vitality, score, false, kItem_NULL },
            { 360, enemy_type,  7, width - 5, height, 268, vitality, score, false, kItem_NULL },
            { 400, enemy_type,  8, width - 5, height, 268, vitality, score, false, kItem_NULL },
            { 440, enemy_type,  9, width - 5, height, 268, vitality, score, false, kItem_NULL },
            { 480, enemy_type, 10, width - 5, height, 268, vitality, score, false, kItem_NULL },
            { 520, enemy_type, 11, width - 5, height, 268, vitality, score, false, kItem_NULL },
            { 560, enemy_type, 12, width - 5, height, 268, vitality, score, false, kItem_NULL },
        };
        for (int i = 0; i < sizeof(com_list) / sizeof(com_list[0]); i++) {
            m_create_enemy_com_list.push_back(com_list[i]);
        }
    }

    if (m_state_count == 2000) {
        const kEnemy enemy_type = kEnemy_03;
        const int vitality = ENEMY_03_VITALITY;
        const int score = ENEMY_03_SCORE;
        CreateEnemyCommand com_list[] = {
            {   0, enemy_type,  1,     0, height * 0.5f,   0, vitality, score, false, kItem_NULL },
            {  40, enemy_type,  2,     0, height * 0.5f,   0, vitality, score, false, kItem_NULL },
            {  80, enemy_type,  3,     0, height * 0.5f,   0, vitality, score, false, kItem_NULL },
            { 120, enemy_type,  4,     0, height * 0.5f,   0, vitality, score, false, kItem_NULL },
            { 160, enemy_type,  5,     0, height * 0.5f,   0, vitality, score, false, kItem_NULL },
            { 200, enemy_type,  6,     0, height * 0.5f,   0, vitality, score, false, kItem_NULL },
            { 360, enemy_type,  7, width, height * 0.5f, 180, vitality, score, false, kItem_NULL },
            { 400, enemy_type,  8, width, height * 0.5f, 180, vitality, score, false, kItem_NULL },
            { 440, enemy_type,  9, width, height * 0.5f, 180, vitality, score, false, kItem_NULL },
            { 480, enemy_type, 10, width, height * 0.5f, 180, vitality, score, false, kItem_NULL },
            { 520, enemy_type, 11, width, height * 0.5f, 180, vitality, score, false, kItem_NULL },
            { 560, enemy_type, 12, width, height * 0.5f, 180, vitality, score, false, kItem_NULL },
        };
        for (int i = 0; i < sizeof(com_list) / sizeof(com_list[0]); i++) {
            m_create_enemy_com_list.push_back(com_list[i]);
        }
    }

    if (m_state_count == 3000) {
        const kEnemy enemy_type = kEnemy_04;
        const int vitality = ENEMY_04_VITALITY;
        const int score = ENEMY_04_SCORE;
        CreateEnemyCommand com_list[] = {
            { 0, enemy_type,  1,  60, height +   0, 270, vitality, score, false, kItem_NULL },
            { 0, enemy_type,  2, 195, height +   0, 270, vitality, score, false, kItem_NULL },
            { 0, enemy_type,  3, 135, height +  35, 270, vitality, score, false, kItem_NULL },
            { 0, enemy_type,  4, 285, height +  35, 270, vitality, score, false, kItem_NULL },
            { 0, enemy_type,  5,  45, height +  70, 270, vitality, score, false, kItem_NULL },
            { 0, enemy_type,  6, 200, height +  70, 270, vitality, score, false, kItem_NULL },
            { 0, enemy_type,  7, 115, height + 105, 270, vitality, score, false, kItem_NULL },
            { 0, enemy_type,  8, 265, height + 105, 270, vitality, score, false, kItem_NULL },
            { 0, enemy_type,  9, 190, height + 130, 270, vitality, score, false, kItem_NULL },
            { 0, enemy_type, 10,  60, height + 140, 270, vitality, score, false, kItem_NULL },

        };
        for (int i = 0; i < sizeof(com_list) / sizeof(com_list[0]); i++) {
            m_create_enemy_com_list.push_back(com_list[i]);
        }
    }

    if (m_state_count == 4100) {
        const kEnemy enemy_type = kEnemy_05;
        const int vitality = ENEMY_05_VITALITY;
        const int score = ENEMY_05_SCORE;
        CreateEnemyCommand com_list[] = {
            {   0, enemy_type, 1, width * 0.5f +  0, height, 270, vitality * 2, score, false, kItem_01 },
            {   0, enemy_type, 2, width * 0.5f - 80, height, 270, vitality * 1, score, false, kItem_NULL },
            {   0, enemy_type, 3, width * 0.5f + 80, height, 270, vitality * 1, score, false, kItem_NULL },
        };
        for (int i = 0; i < sizeof(com_list) / sizeof(com_list[0]); i++) {
            m_create_enemy_com_list.push_back(com_list[i]);
        }
    }

    if (m_state_count == 5700) {
        const kEnemy enemy_type = kEnemy_06;
        const int vitality = ENEMY_06_VITALITY;
        const int score = ENEMY_06_SCORE;
        CreateEnemyCommand com_list[] = {
            {   0, enemy_type, 1,  width * 0.5f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            {   0, enemy_type, 2,             0, height * 0.5f,   0, vitality, score, false, kItem_NULL },
            {   0, enemy_type, 3,  width * 1.0f, height * 0.6f, 180, vitality, score, false, kItem_NULL },
            {  95, enemy_type, 4,  width * 0.5f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            {  95, enemy_type, 5,             0, height * 0.5f,   0, vitality, score, false, kItem_NULL },
            {  95, enemy_type, 6,  width * 1.0f, height * 0.6f, 180, vitality, score, false, kItem_NULL },
            { 180, enemy_type, 7,  width * 0.5f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 180, enemy_type, 8,             0, height * 0.5f,   0, vitality, score, false, kItem_NULL },
            { 180, enemy_type, 9,  width * 1.0f, height * 0.6f, 180, vitality, score, false, kItem_NULL },
            { 265, enemy_type, 10, width * 0.5f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 265, enemy_type, 11,            0, height * 0.5f,   0, vitality, score, false, kItem_NULL },
            { 265, enemy_type, 12, width * 1.0f, height * 0.6f, 180, vitality, score, false, kItem_NULL },
        };
        for (int i = 0; i < sizeof(com_list) / sizeof(com_list[0]); i++) {
            m_create_enemy_com_list.push_back(com_list[i]);
        }
    }

    if (m_state_count == 6900) {
        const kEnemy enemy_type = kEnemy_07;
        const int vitality = ENEMY_07_VITALITY;
        const int score = ENEMY_07_SCORE;
        CreateEnemyCommand com_list[] = {
            {   0, enemy_type,  1, width * 0.3f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            {  40, enemy_type,  2, width * 0.0f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            {  70, enemy_type,  3, width * 0.7f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 100, enemy_type,  4, width * 0.9f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 130, enemy_type,  5, width * 0.3f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 150, enemy_type,  6, width * 0.1f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 200, enemy_type,  7, width * 0.4f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 240, enemy_type,  8, width * 0.8f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 280, enemy_type,  9, width * 0.3f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 300, enemy_type, 10, width * 0.5f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 325, enemy_type, 11, width * 0.1f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 325, enemy_type, 12, width * 0.9f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 430, enemy_type, 13, width * 0.0f, height * 0.8f,   0, vitality, score, false, kItem_NULL },
            { 450, enemy_type, 14, width * 1.0f, height * 0.6f, 180, vitality, score, false, kItem_NULL },
            { 460, enemy_type, 15, width * 0.8f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 480, enemy_type, 16, width * 0.3f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 500, enemy_type, 17, width * 1.0f, height * 0.6f, 180, vitality, score, false, kItem_NULL },
            { 510, enemy_type, 18, width * 0.1f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 520, enemy_type, 19, width * 0.3f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 530, enemy_type, 20, width * 0.9f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 540, enemy_type, 21, width * 0.0f, height * 0.8f,   0, vitality, score, false, kItem_NULL },
            { 550, enemy_type, 22, width * 1.0f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 560, enemy_type, 23, width * 0.3f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 570, enemy_type, 24, width * 0.7f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 580, enemy_type, 25, width * 0.1f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 590, enemy_type, 26, width * 0.9f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 600, enemy_type, 27, width * 0.1f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 610, enemy_type, 28, width * 0.0f, height * 0.8f,   0, vitality, score, false, kItem_NULL },
            { 620, enemy_type, 29, width * 0.0f, height * 0.7f,   0, vitality, score, false, kItem_NULL },
            { 630, enemy_type, 30, width * 1.0f, height * 0.8f, 180, vitality, score, false, kItem_NULL },
            { 640, enemy_type, 31, width * 0.1f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 650, enemy_type, 32, width * 0.6f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 660, enemy_type, 33, width * 0.7f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 670, enemy_type, 34, width * 0.2f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 680, enemy_type, 35, width * 0.5f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 690, enemy_type, 36, width * 0.9f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 700, enemy_type, 37, width * 0.3f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 750, enemy_type, 38, width * 0.7f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 760, enemy_type, 39, width * 0.1f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 770, enemy_type, 40, width * 0.5f, height * 1.0f, 270, vitality, score, false, kItem_NULL },
            { 780, enemy_type, 41, width * 0.0f, height * 0.8f, 270, vitality, score, false, kItem_NULL },
            { 790, enemy_type, 42, width * 1.0f, height * 0.7f, 270, vitality, score, false, kItem_NULL },
        };
        for (int i = 0; i < sizeof(com_list) / sizeof(com_list[0]); i++) {
            m_create_enemy_com_list.push_back(com_list[i]);
        }
    }

    if (m_state_count == 8300) {
        auto game_layer = GameLayer::getGameLayer();
        game_layer->stopBackgroundScroll();
        game_layer->showBossMessage();

        if (GameLayer::getBackGroundMusic()) {
            if (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()) {
                CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
            }
            CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.1f);
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(BACK_GROUND_MUSIC_BOSS_01, false);
        }

        CreateEnemyCommand com_list[] = {
            { 100, kEnemyBoss_01, 1, width * 0.5f, height + 50, 270, ENEMY_BOSS_01_VITALITY, ENEMY_BOSS_01_SCORE, false, kItem_NULL },
        };
        for (int i = 0; i < sizeof(com_list) / sizeof(com_list[0]); i++) {
            m_create_enemy_com_list.push_back(com_list[i]);
        }
    }

    createEnemyByCommand(enemy_list);
    m_state_count++;
}

void EnemyControllerState02::createEnemy(const EnemyController* context, int count, std::list<Enemy*>& enemy_list)
{
}

std::shared_ptr<EnemyControllerState> EnemyControllerState01::nextState(const EnemyController* context) const
{
    if (context->getBossDestroyed()) {
        return std::make_shared<EnemyControllerState02>(kGameStage_End);
    } else {
        return std::shared_ptr<EnemyControllerState>();
    }
}

std::shared_ptr<EnemyControllerState> EnemyControllerState02::nextState(const EnemyController* context) const
{
    return std::shared_ptr<EnemyControllerState>();
}
