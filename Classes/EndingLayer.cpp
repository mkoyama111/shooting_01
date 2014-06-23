#include "GameConfig.h"
#include "TitleLayer.h"
#include "EndingLayer.h"
#include "GameData.h"

USING_NS_CC;

Scene* EndingLayer::scene()
{
    Scene* scene = Scene::create();
    EndingLayer* layer = EndingLayer::create();
    scene->addChild(layer);
    return scene;
}

bool EndingLayer::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    const Size winSize = Director::getInstance()->getWinSize();

    const float delay_time = 1.5f;
    const float fade_in_time = 3.0f;

#ifdef WIN32
    char buf[128];
#endif

    //// Game Clear
    {
#ifdef WIN32
        ::WideCharToMultiByte(CP_UTF8, 0, L"ゲームクリア！", -1, buf, sizeof(buf), NULL, NULL);
        auto game_clear_label = Label::createWithBMFont(BM_FONT_01, buf);
#else
        auto game_clear_label = Label::createWithBMFont(BM_FONT_01, "ゲームクリア！");
#endif
        game_clear_label->setPosition(Point(winSize.width * 0.5f, winSize.height * 0.7f));
        game_clear_label->setColor(Color3B(255, 255, 255));
        game_clear_label->setOpacity(0);
        this->addChild(game_clear_label);
        game_clear_label->runAction(CCSequence::create(
            CCDelayTime::create(delay_time),
            CCFadeIn::create(fade_in_time),
            NULL));
    }

    //// Score
    {
#ifdef WIN32
        ::WideCharToMultiByte(CP_UTF8, 0, L"スコア:", -1, buf, sizeof(buf), NULL, NULL);
        auto game_score_label = Label::createWithBMFont(BM_FONT_01, buf);
#else
        auto game_score_label = Label::createWithBMFont(BM_FONT_01, "スコア:");
#endif
        game_score_label->setPosition(Point(winSize.width * 0.40f, winSize.height * 0.55f));
        game_score_label->setColor(Color3B(255, 255, 255));
        game_score_label->setOpacity(0);
        this->addChild(game_score_label);
        game_score_label->runAction(CCSequence::create(
            CCDelayTime::create(delay_time),
            CCFadeIn::create(fade_in_time),
            NULL));

        const int score_val = GameData::getInstance()->getScore(); 
        TTFConfig ttf_config(TTF_FONT_01, 24);
        auto score_val_label = Label::createWithTTF(ttf_config, "0");
        score_val_label->setPosition(Point(winSize.width * 0.65f, winSize.height * 0.55f));
        score_val_label->setString(CCString::createWithFormat("%d", score_val)->getCString());
        this->addChild(score_val_label);
    }

    //// Back to title
    {
#ifdef WIN32
        ::WideCharToMultiByte(CP_UTF8, 0, L"タイトルにもどる", -1, buf, sizeof(buf), NULL, NULL);
        auto back_label = Label::createWithBMFont(BM_FONT_01, buf);
#else
        auto back_label = Label::createWithBMFont(BM_FONT_01, "タイトルにもどる");
#endif
        back_label->setColor(Color3B(255, 255, 255));
        back_label->setOpacity(0);

        MenuItemLabel* back_btn = MenuItemLabel::create(back_label, CC_CALLBACK_1(EndingLayer::backToTitle, this));
        back_btn->setPosition(Point(winSize.width * 0.5f, winSize.height * 0.40f));
        Menu* menu = Menu::create(back_btn, NULL);

        menu->setPosition(Point::ZERO);
        this->addChild(menu);
        back_label->runAction(CCSequence::create(
            CCDelayTime::create(delay_time),
            CCFadeIn::create(fade_in_time),
            NULL));
    }

    //// BGM
    {
#ifdef WIN32
        ::WideCharToMultiByte(CP_UTF8, 0, L"BGM: 煉獄庭園様", -1, buf, sizeof(buf), NULL, NULL);
        auto bgm_label = Label::createWithBMFont(BM_FONT_01, buf);
#else
        auto bgm_label = Label::createWithBMFont(BM_FONT_01, "BGM: 煉獄庭園様");
#endif
        bgm_label->setPosition(Point(winSize.width * 0.68f, winSize.height * 0.1f));
        bgm_label->setColor(Color3B(255, 255, 255));
        bgm_label->setOpacity(0);
        this->addChild(bgm_label);
        bgm_label->runAction(CCSequence::create(
            CCDelayTime::create(delay_time),
            CCFadeIn::create(fade_in_time),
            NULL));
    }

    return true;
}

void EndingLayer::backToTitle(cocos2d::Ref* pSender)
{
    Scene* scene = TitleLayer::scene();
    TransitionFade* transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
}
