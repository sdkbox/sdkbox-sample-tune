
#include "HelloWorldScene.h"
#include "PluginTune/PluginTune.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    CCLOG("Sample Startup");

    // add logo
    auto winsize = Director::getInstance()->getWinSize();
    auto logo = Sprite::create("Logo.png");
    auto logoSize = logo->getContentSize();
    logo->setPosition(Vec2(logoSize.width / 2,
                           winsize.height - logoSize.height / 2));
    addChild(logo);

    // add quit button
    auto label = Label::createWithSystemFont("QUIT", "sans", 32);
    auto quit = MenuItemLabel::create(label, [](Ref*){
        exit(0);
    });
    auto labelSize = label->getContentSize();
    quit->setPosition(Vec2(winsize.width / 2 - labelSize.width / 2 - 16,
                           -winsize.height / 2 + labelSize.height / 2 + 16));
    addChild(Menu::create(quit, NULL));

    // add test menu
    createTestMenu();

    return true;
}

void HelloWorld::createTestMenu()
{
    sdkbox::PluginTune::setDebugMode(true);
    sdkbox::PluginTune::measureSession();
    sdkbox::PluginTune::setListener(this);
    
    sdkbox::PluginTune::checkForDeferredDeepLink();
    

    auto menu = Menu::create();

    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("purchase event", "sans", 24), [](Ref*){
        CCLOG("purchase event");
        sdkbox::PluginTune::measureEventName("purchase");
        sdkbox::PluginTune::measureEventId(1122334455);
        sdkbox::TuneEvent event;
        event.eventName = "purchase2";
        event.refId     = "RJ1357";
        event.searchString = "sweet crisp red apples";
        event.attribute1 = "crisp";
        event.attribute2 = "red";
        event.quantity = 3;
        sdkbox::PluginTune::measureEvent(event);
    }));

    menu->alignItemsVerticallyWithPadding(10);
    addChild(menu);
}

void HelloWorld::onMobileAppTrackerEnqueuedActionWithReferenceId(const std::string &referenceId)
{
    CCLOG("onMobileAppTrackerEnqueuedActionWithReferenceId, referenceId = %s", referenceId.c_str());
}

void HelloWorld::onMobileAppTrackerDidSucceedWithData(const std::string &data)
{
    CCLOG("onMobileAppTrackerDidSucceedWithData, data = %s", data.c_str());
}

void HelloWorld::onMobileAppTrackerDidFailWithError(const std::string &errorString)
{
    CCLOG("onMobileAppTrackerDidFailWithError, error = %s", errorString.c_str());
}

void HelloWorld::onMobileAppTrackerDidReceiveDeeplink(const std::string &deeplink, bool timeout)
{
    CCLOG("onMobileAppTrackerDidReceiveDeeplink, deepLink = %s, timeout = %s", deeplink.c_str(), timeout ? "yes" : "no");
}

void HelloWorld::onMobileAppTrackerDidFailDeeplinkWithError(const std::string &errorString)
{
    CCLOG("onMobileAppTrackerDidFailDeeplinkWithError, error = %s", errorString.c_str());
}
