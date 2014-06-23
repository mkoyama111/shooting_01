#include <stddef.h>
#include "GameData.h"

GameData::GameData() : m_score(0)
{
}

GameData* GameData::getInstance()
{
    static GameData* s_instance = NULL;
    if (!s_instance) {
        s_instance = new GameData;
    }
    return s_instance;
}

int GameData::getScore()
{
    return m_score;
}

void GameData::setScore(int score)
{
    m_score = score;
}