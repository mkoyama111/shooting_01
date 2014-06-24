#include "GameConfig.h"
#include "SimpleAudioEngine.h"
#include "TitleLayer.h"
#include "GameLayer.h"

USING_NS_CC;
using namespace CocosDenshion;

//#pragma execution_character_set("utf-8")

Scene* TitleLayer::scene()
{
    auto scene = Scene::create();
    auto layer = TitleLayer::create();
    scene->addChild(layer);
    return scene;
}

bool TitleLayer::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    const Size winSize = Director::getInstance()->getWinSize();

    showHighScore();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    // Visual Studio 2012 では #pragma execution_character_set("utf-8") 
    // がサポートされていないため、ワイド文字からUTF-8に変換して、
    // CCLabelBMFont::create()にUTF-8文字列を渡す。
    char buf[128];
    ::WideCharToMultiByte(CP_UTF8, 0, L"ゲームスタート", -1, buf, sizeof(buf), NULL, NULL);
    auto start_label =  Label::createWithBMFont(BM_FONT_01, buf);
#else
    auto start_label = Label::createWithBMFont(BM_FONT_01, "ゲームスタート");
#endif
    start_label->setColor(Color3B(255, 255, 255));
    auto start_btn = MenuItemLabel::create(start_label, CC_CALLBACK_1(TitleLayer::gameStartCallback, this));        
    start_btn->setPosition(Point(winSize.width * 0.5f, winSize.height * 0.7f));

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    ::WideCharToMultiByte(CP_UTF8, 0, L"ゲーム終了", -1, buf, sizeof(buf), NULL, NULL);
    auto end_label = Label::createWithBMFont(BM_FONT_01, buf);
#else
    auto end_label = Label::createWithBMFont(BM_FONT_01, "ゲーム終了");
#endif
    end_label->setColor(Color3B(255, 255, 255));
    auto end_btn = MenuItemLabel::create(end_label, CC_CALLBACK_1(TitleLayer::gameEndCallback, this));        
    end_btn->setPosition(Point(winSize.width * 0.5f, winSize.height * 0.45f));
   
    auto menu = Menu::create(start_btn, end_btn, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);

    if (GameLayer::getBackGroundMusic()) {
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(BACK_GROUND_MUSIC_01);
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(BACK_GROUND_MUSIC_BOSS_01);
    }

    return true;
}

void TitleLayer::gameStartCallback(cocos2d::Ref* pSender)
{
    auto scene = GameLayer::scene();
    auto transition = TransitionFade::create(0.5f, scene);
    Director::getInstance()->replaceScene(transition);
}

void TitleLayer::gameEndCallback(cocos2d::Ref* pSender)
{
    Director::getInstance()->end();
}

void TitleLayer::showHighScore()
{
    const Size winSize = Director::getInstance()->getWinSize();
    const int high_score_val = UserDefault::getInstance()->getIntegerForKey("HighScore", 0);

    TTFConfig ttf_config(TTF_FONT_01, 18);

    auto lbl_hi_score = Label::createWithTTF(ttf_config, "Hi-Score:");
    lbl_hi_score->setPosition(Point(235, winSize.height * 0.95));
    lbl_hi_score->setAnchorPoint(Point(1, 0));
    this->addChild(lbl_hi_score);

    auto hi_score = Label::createWithTTF(ttf_config, "0");
    hi_score->setPosition(Point(315, winSize.height * 0.95));
    hi_score->setAnchorPoint(Point(1, 0));
    hi_score->setString(String::createWithFormat("%d", high_score_val)->getCString());
    this->addChild(hi_score);
}
