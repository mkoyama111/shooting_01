#ifndef ENEMY_H__
#define ENEMY_H__

#include <list>
#include <vector>
#include "cocos2d.h"
#include "GameConfig.h"

class EnemyBullet;

class Enemy : public cocos2d::Sprite
{
public:
    Enemy(kEnemy type, int num, float x, float y, int angle, int vitality, int score, bool on_bg, kItem item);
    virtual ~Enemy();

    CC_SYNTHESIZE_READONLY(bool, m_finished, Finished);
    CC_SYNTHESIZE_READONLY(kEnemy, m_enemy_type, EnemyType);
    CC_SYNTHESIZE_READONLY(int, m_vitality, Vitality);
    CC_SYNTHESIZE_READONLY(int, m_score, Score);
    CC_SYNTHESIZE_READONLY(int, m_on_background, OnBackground);
    CC_SYNTHESIZE_READONLY(bool, m_is_item, IsItem);
    CC_SYNTHESIZE_READONLY(cocos2d::Sprite*, m_shadow, Shadow);
    CC_SYNTHESIZE(bool, m_laser_locked, LaserLocked);
    CC_SYNTHESIZE(cocos2d::Sprite*, m_laser_lock_mark, LaserLockMark);

    static Enemy* createEnemy(kEnemy type, int num, float x, float y, int angle, int vitality, int score, bool on_bg, kItem item);

    virtual void update(float dt);

    virtual bool damage(int point);

protected:
    struct MoveCommand {
        float x;
        float y;
        int angle;
        float speed;
        float angle_speed;
        bool smooth;
        bool sine_curve;
        int time_count;
    };

    struct CreateBulletCommand {
        kEnemyBullet bullet_type;
        float x;
        float y;
        short angle;
        float speed;
        float accel;
        float speed_end;
        short bullets_count;
        short distance_between_bullets;
        short stop_time;
        short move_time;
        short stop_time_2;
        short angle_2;
        float speed_2;
        float accel_2;
        float speed_end_2;
        short vanish_time;
        short count_of_shot;
        short time_to_start;
        short time_to_next_shot;
        short angle_rate;
    };

    static Enemy* spriteWithFile(kEnemy type, int num, float x, float y, int angle, int vitality, int score, bool on_bg, kItem item);

    void createBulletsByCommand();

    void createStraightLineBullets(const CreateBulletCommand& com);

    void FinishSelf();

    cocos2d::Sprite* createLifeGage();

    void setDxDyByMoveCommand();

    void moveBy(float x, float y, int time_count, bool smooth);
    void moveTo(float x, float y, int time_count, bool smooth);
    void stopMove(int time_count);
    void moveWithAngleSpeed(int angle, float speed, float angle_speed, int time_count);
    void moveSineCurve(int angle, float speed, float width_x, float width_y, float angle_speed, int time_count);
    void emitItem();

    virtual void update2() = 0;

    std::list<MoveCommand> m_move_com_list;
    std::list<CreateBulletCommand> m_create_bullets_com_list;
    cocos2d::Point m_shadow_offset;
    cocos2d::Size m_winSize;
    int m_num;
    int m_angle;
    float m_angle_f;
    float m_speed;
    float m_accel;
    int m_count;
    float m_dx;
    float m_dy;
    kItem m_item;
};

#endif
