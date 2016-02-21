#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "PluginTune/PluginTune.h"

class HelloWorld : public cocos2d::Layer, public sdkbox::TuneListener
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:
    void createTestMenu();

    virtual void onMobileAppTrackerEnqueuedActionWithReferenceId(const std::string &referenceId);
    virtual void onMobileAppTrackerDidSucceedWithData(const std::string &data);
    virtual void onMobileAppTrackerDidFailWithError(const std::string &errorString);
    virtual void onMobileAppTrackerDidReceiveDeeplink(const std::string &deeplink, bool timeout);
    virtual void onMobileAppTrackerDidFailDeeplinkWithError(const std::string &errorString);
};

#endif // __HELLOWORLD_SCENE_H__
