#ifndef ENEMY_CONTROLLER_H__
#define ENEMY_CONTROLLER_H__

#include <list>
#include <memory>
#include "cocos2d.h"
#include "GameConfig.h"

class Enemy;
class EnemyController;

enum kGameStage {
    kGameStage_01,
    kGameStage_End,
};

class EnemyControllerState
{
public:
    EnemyControllerState(kGameStage game_stage);

    CC_SYNTHESIZE(kGameStage, m_game_stage, GameStage);

    virtual void createEnemy(const EnemyController* context, int count, std::list<Enemy*>& enemy_list) = 0;
    virtual std::shared_ptr<EnemyControllerState> nextState(const EnemyController* context) const = 0;

protected:
    struct CreateEnemyCommand {
        int count_to_start;
        kEnemy type;
        int num;
        float x;
        float y;
        int angle;
        int vitality;
        int score;
        bool on_background;
        kItem item;
    };

    void createEnemyByCommand(std::list<Enemy*>& enemy_list);

    std::list<CreateEnemyCommand> m_create_enemy_com_list;

    cocos2d::Size m_winSize;
    int m_state_count;
};

class EnemyController
{
public:
    EnemyController();

    CC_SYNTHESIZE(bool, m_boss_destroyed, BossDestroyed);

    kGameStage getGameStage() const;

    void createEnemies(std::list<Enemy*>& enemy_list);

protected:
    std::shared_ptr<EnemyControllerState> m_state;
    int m_count;
};

#endif
