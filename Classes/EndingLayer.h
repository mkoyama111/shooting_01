#ifndef ENDING_LAYER_H_
#define ENDING_LAYER_H_

#include "cocos2d.h"

class EndingLayer : public cocos2d::CCLayer
{
public:
    virtual bool init();
    static cocos2d::Scene* scene();
    CREATE_FUNC(EndingLayer);

protected:
    void backToTitle(cocos2d::Ref* pSender);
};

#endif
