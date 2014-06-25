#ifndef GAME_CONFIG_H__
#define GAME_CONFIG_H__

#include "cocos2d.h"

#define IMAGE_BACKGROUND             "back01.png"
#define IMAGE_MY_SHIP_01             "my_ship_01.png"
#define IMAGE_MY_SHIP_02             "my_ship_02.png"
#define IMAGE_MY_BULLET_01           "my_bullet_01.png"
#define IMAGE_MY_BULLET_02           "my_bullet_02.png"
#define IMAGE_MY_BULLET_03           "my_bullet_03.png"
#define IMAGE_MY_BULLET_LASER        "my_laser_01.png"
#define IMAGE_ENEMY_01               "enemy_01.png"
#define IMAGE_ENEMY_02               "enemy_01.png"
#define IMAGE_ENEMY_03               "enemy_01.png"
#define IMAGE_ENEMY_04               "enemy_04.png"
#define IMAGE_ENEMY_05_1             "enemy_05_1.png"
#define IMAGE_ENEMY_05_2             "enemy_05_2.png"
#define IMAGE_ENEMY_06               "enemy_04.png"
#define IMAGE_ENEMY_07               "enemy_01.png"
#define IMAGE_ENEMY_08               "enemy_08.png"
#define IMAGE_ENEMY_09               "enemy_09.png"
#define IMAGE_ENEMY_GROUND_01        "enemy_ground_01.png"
#define IMAGE_ENEMY_BOSS_01          "enemy_boss_01.png"
#define IMAGE_ENEMY_ITEM_01          "item_01.png"
#define IMAGE_DAMAGE_ENEMY_01        "damage_enemy_01.png"
#define IMAGE_DAMAGE_ENEMY_02        "damage_enemy_01.png"
#define IMAGE_DAMAGE_ENEMY_03        "damage_enemy_01.png"
#define IMAGE_DAMAGE_ENEMY_04        "damage_enemy_04.png"
#define IMAGE_DAMAGE_ENEMY_05_1      "damage_enemy_05_1.png"
#define IMAGE_DAMAGE_ENEMY_05_2      "damage_enemy_05_2.png"
#define IMAGE_DAMAGE_ENEMY_06        "damage_enemy_04.png"
#define IMAGE_DAMAGE_ENEMY_07        "damage_enemy_01.png"
#define IMAGE_DAMAGE_ENEMY_08        "damage_enemy_08.png"
#define IMAGE_DAMAGE_ENEMY_09        "damage_enemy_09.png"
#define IMAGE_DAMAGE_ENEMY_GROUND_01 "damage_enemy_ground_01.png"
#define IMAGE_DAMAGE_ENEMY_BOSS_01   "damage_enemy_boss_01.png"
#define IMAGE_ENEMY_BULLET_01        "enemy_bullet_01.png"
#define IMAGE_ENEMY_BULLET_02        "enemy_bullet_02.png"
#define IMAGE_ENEMY_BULLET_03        "enemy_bullet_03.png"
#define IMAGE_ENEMY_BULLET_04        "enemy_bullet_04.png"
#define IMAGE_ENEMY_BULLET_05        "enemy_bullet_05.png"
#define IMAGE_ENEMY_BULLET_06        "enemy_bullet_06.png"
#define IMAGE_MY_LASER_LOCK          "my_laser_lock_01.png"
#define IMAGE_MY_SHIP_LIFE_GAGE      "my_ship_life_gage_01.png"
#define IMAGE_MY_SHIP_LIFE_GAGE_BG   "my_ship_life_gage_bg.png"
#define IMAGE_BOSS_LIFE_GAGE         "boss_life_gage_01.png"
#define IMAGE_BOSS_LIFE_GAGE_BG      "boss_life_gage_bg.png"

#define SE_MY_SHIP_EXPLOSION         "explosion_01.wav"
#define SE_ENEMY_EXPLOSION           "explosion_02.wav"
#define SE_ENEMY_BOSS_EXPLOSION      "explosion_03.wav"
#define SE_MY_LASER_LOCK             "my_laser_lock_01.wav"
#define SE_GET_ITEM_01               "get_item_01.wav"

#define ANIME_DAMAGE_ENEMY_01        "anime_damage_enemy_01"
#define ANIME_DAMAGE_ENEMY_02        "anime_damage_enemy_02"
#define ANIME_DAMAGE_ENEMY_03        "anime_damage_enemy_03"
#define ANIME_DAMAGE_ENEMY_04        "anime_damage_enemy_04"
#define ANIME_DAMAGE_ENEMY_05_1      "anime_damage_enemy_05_1"
#define ANIME_DAMAGE_ENEMY_05_2      "anime_damage_enemy_05_2"
#define ANIME_DAMAGE_ENEMY_06        "anime_damage_enemy_06"
#define ANIME_DAMAGE_ENEMY_07        "anime_damage_enemy_07"
#define ANIME_DAMAGE_ENEMY_08        "anime_damage_enemy_08"
#define ANIME_DAMAGE_ENEMY_09        "anime_damage_enemy_09"
#define ANIME_DAMAGE_ENEMY_GROUND_01 "anime_damage_enemy_ground_01"
#define ANIME_DAMAGE_ENEMY_BOSS_01   "anime_damage_enemy_boss_01"

#define BACK_GROUND_MUSIC_01         "bgm/happypop.mp3"
#define BACK_GROUND_MUSIC_BOSS_01    "bgm/sixth_sense.mp3"

#if ((CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID))
#define BM_FONT_01                   "fonts/font1.fnt"
#define TTF_FONT_01                  "fonts/arial.ttf"
#else
#define BM_FONT_01                   "font1.fnt"
#define TTF_FONT_01                  "arial.ttf"
#endif

// BGMの設定(0:無効, 1:有効)
static const int BACK_GROUND_MUSIC = 1;

enum kMyBullet {
    kMyBullet_01 = 0,
    kMyBullet_02,
    kMyBullet_03,
    kMyBullet_Num,
};

enum kMyLaser {
    kMyLaser_01,
    kMyLaser_Num,
};

enum kEnemy {
    kEnemy_01 = 0,
    kEnemy_02,
    kEnemy_03,
    kEnemy_04,
    kEnemy_05,
    kEnemy_06,
    kEnemy_07,
    kEnemy_08,
    kEnemy_09,
    kEnemyGround_01,
    kEnemyBoss_01,
    kEnemyItem_01,
};

enum kEnemyBullet {
    kEnemyBullet_01 = 0,
    kEnemyBullet_02,
    kEnemyBullet_03,
    kEnemyBullet_04,
    kEnemyBullet_05,
    kEnemyBullet_06,
    kEnemyBullet_Num,
};

enum kItem {
    kItem_NULL = 0,
    kItem_01
};

// 自機
static const float MY_SHIP_SPEED_X    = 5.0f;
static const float MY_SHIP_SPEED_Y    = 5.0f;
static const int MY_SHIP_LIFE_COUNT   = 10;
static const int MY_SHIP_VITALITY     = 1;
static const int MY_SHIP_CRASH_DAMAGE = 1;
static const int MY_SHIP_MUTEKI_TIME_AFTER_DAMAGE = 180;

// 自弾01
static const float MY_BULLET_01_SPEED = 10.0f;
static const int MY_BULLET_01_DAMAGE  = 1;
// 自弾02
static const float MY_BULLET_02_SPEED = 10.0f;
static const int MY_BULLET_02_DAMAGE  = 1;
// 自弾02
static const float MY_BULLET_03_SPEED = 10.0f;
static const int MY_BULLET_03_DAMAGE  = 1;

// 自レーザー
static const float MY_LASER_LOCKING_DISTANCE = 300.0f;
static const float MY_LASER_FADE_TIME        = 0.5f;
static const float MY_LASER_STROKE_WIDTH     = 9.0f;
static const float MY_LASER_SPLINE_DURATION  = 0.8f;
static const int DURATION_OF_SET_MY_LASER    = 60;
static const int MY_LASER_DAMAGE             = 20;

// 敵機01
static const int ENEMY_01_VITALITY = 10;
static const int ENEMY_01_SCORE    = 100;
// 敵機02
static const int ENEMY_02_VITALITY = 10;
static const int ENEMY_02_SCORE    = 100;
// 敵機03
static const int ENEMY_03_VITALITY = 10;
static const int ENEMY_03_SCORE    = 100;
// 敵機04
static const int ENEMY_04_VITALITY = 50;
static const int ENEMY_04_SCORE    = 500;
// 敵機05
static const int ENEMY_05_VITALITY = 150;
static const int ENEMY_05_SCORE    = 1500;
// 敵機06
static const int ENEMY_06_VITALITY = 50;
static const int ENEMY_06_SCORE    = 500;
// 敵機07
static const int ENEMY_07_VITALITY = 8;
static const int ENEMY_07_SCORE    = 100;
// 敵機08
static const int ENEMY_08_VITALITY = 200;
static const int ENEMY_08_SCORE    = 2000;
// 敵機09
static const int ENEMY_09_VITALITY = 100;
static const int ENEMY_09_SCORE    = 1000;
// 地上敵01
static const int ENEMY_GROUND_01_VITALITY = 10;
static const int ENEMY_GROUND_01_SCORE    = 100;
// 敵ボス01
static const int ENEMY_BOSS_01_VITALITY = 2000;
static const int ENEMY_BOSS_01_SCORE    = 5000;

#endif
