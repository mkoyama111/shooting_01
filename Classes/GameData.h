#ifndef GAME_DATA_H__
#define GAME_DATA_H__

class GameData
{
public:
    GameData();
    static GameData* getInstance();

    int getScore();
    void setScore(int score);
private:
    int m_score;
};

#endif
