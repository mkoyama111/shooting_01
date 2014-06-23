#ifndef TITLE_LAYER_H__
#define TITLE_LAYER_H__

#include "cocos2d.h"

class TitleLayer : public cocos2d::CCLayer
{
public:
    virtual bool init();
    static cocos2d::Scene* scene();
    CREATE_FUNC(TitleLayer);

protected:
    void gameStartCallback(cocos2d::Ref* pSender);
    void gameEndCallback(cocos2d::Ref* pSender);
    void showHighScore();
};

#endif
