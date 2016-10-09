#ifndef __TUNE_WRAPPER_H__
#define __TUNE_WRAPPER_H__

#include "CocosMacros.h"
#include "json98.h"
#include "PluginTune.h"

NS_COCOS_BEGIN

#define TAG   "Tune"
#define VERSION "3.15.0"
#define IOS_VERSION     "ios: 3.15.0"
#define ANDROID_VERSION "android: 3.11.4"

class TuneWrapper {
public:
    static TuneWrapper* getInstance();
    
    virtual void setListener(TuneListener* listener)=0;
    virtual TuneListener* getListener()=0;
    virtual void removeListener()=0;
    virtual void init()=0;
    virtual void setDebugMode(bool enable)=0;
    virtual void setAllowDuplicateRequests(bool allow)=0;
    virtual void checkForDeferredDeeplinkWithTimeout(double timeout)=0;
    virtual void checkForDeferredDeepLink()=0;
    virtual void automateIapEventMeasurement(bool automate)=0;
    virtual void setFacebookEventLogging(bool logging, bool limit)=0;
    virtual void setExistingUser(bool existingUser)=0;
    virtual void setAppleAdvertisingIdentifier(const std::string &appleAdvertisingIdentifier, bool adTrackingEnabled)=0;
    virtual void setAppleVendorIdentifier(const std::string &appleVendorIdentifier)=0;
    virtual void setCurrencyCode(const std::string &currencyCode)=0;
    virtual void setJailbroken(bool jailbroken)=0;
    virtual void setPackageName(const std::string &packageName)=0;
    virtual void setShouldAutoDetectJailbroken(bool autoDetect)=0;
    virtual void setShouldAutoGenerateAppleVendorIdentifier(bool autoGenerate)=0;
    virtual void setSiteId(const std::string &siteId)=0;
    virtual void setTRUSTeId(const std::string &tpid)=0;
    virtual void setUserEmail(const std::string &userEmail)=0;
    virtual void setUserId(const std::string &userId)=0;
    virtual void setUserName(const std::string &userName)=0;
    virtual void setPhoneNumber(const std::string &phoneNumber)=0;
    virtual void setFacebookUserId(const std::string &facebookUserId)=0;
    virtual void setTwitterUserId(const std::string &twitterUserId)=0;
    virtual void setGoogleUserId(const std::string &googleUserId)=0;
    virtual void setAge(int userAge)=0;
    virtual void setGender(PluginTune::Gender userGender)=0;
    virtual void setLatitude(double latitude, double longitude)=0;
    virtual void setLatitude(double latitude, double longitude, double altitude)=0;
    virtual void setAppAdTracking(bool enable)=0;
    virtual void setPayingUser(bool isPayingUser)=0;
    virtual void setPreloadData(const TunePreloadData &preloadData)=0;
    virtual void setPreloadDataForScript(const std::string &jsonString)=0;
    virtual std::string matId()=0;
    virtual std::string tuneId()=0;
    virtual std::string openLogId()=0;
    virtual bool isPayingUser()=0;
    virtual void measureSession()=0;
    virtual void measureEventName(const std::string &eventName)=0;
    virtual void measureEventId(int eventId)=0;
    virtual void measureEvent(const TuneEvent &event)=0;
    virtual void measureEventForScript(const std::string &jsonString)=0;
    virtual void setUseCookieTracking(bool enable)=0;
    virtual void setRedirectUrl(const std::string &redirectUrl)=0;
    virtual void startAppToAppTracking(const std::string &targetAppPackageName,
                               const std::string &targetAppAdvertiserId,
                               const std::string &targetAdvertiserOfferId,
                               const std::string &targetAdvertiserPublisherId,
                               bool shouldRedirect)=0;
    virtual void applicationDidOpenURL(const std::string &urlString, const std::string &sourceApplication)=0;
    virtual void setDeepLink(const std::string &deepLinkUrl)=0;
};

class TuneWrapperDisabled : public TuneWrapper {
public:
    void setListener(TuneListener* listener) {}
    TuneListener* getListener() { return NULL; }
    void removeListener() {}
    void init() {}
    void setDebugMode(bool enable) {}
    void setAllowDuplicateRequests(bool allow) {}
    void checkForDeferredDeeplinkWithTimeout(double timeout) {}
    void checkForDeferredDeepLink() {}
    void automateIapEventMeasurement(bool automate) {}
    void setFacebookEventLogging(bool logging, bool limit) {}
    void setExistingUser(bool existingUser) {}
    void setAppleAdvertisingIdentifier(const std::string &appleAdvertisingIdentifier, bool adTrackingEnabled) {}
    void setAppleVendorIdentifier(const std::string &appleVendorIdentifier) {}
    void setCurrencyCode(const std::string &currencyCode) {}
    void setJailbroken(bool jailbroken) {}
    void setPackageName(const std::string &packageName) {}
    void setShouldAutoDetectJailbroken(bool autoDetect) {}
    void setShouldAutoGenerateAppleVendorIdentifier(bool autoGenerate) {}
    void setSiteId(const std::string &siteId) {}
    void setTRUSTeId(const std::string &tpid) {}
    void setUserEmail(const std::string &userEmail) {}
    void setUserId(const std::string &userId) {}
    void setUserName(const std::string &userName) {}
    void setPhoneNumber(const std::string &phoneNumber) {}
    void setFacebookUserId(const std::string &facebookUserId) {}
    void setTwitterUserId(const std::string &twitterUserId) {}
    void setGoogleUserId(const std::string &googleUserId) {}
    void setAge(int userAge) {}
    void setGender(PluginTune::Gender userGender) {}
    void setLatitude(double latitude, double longitude) {}
    void setLatitude(double latitude, double longitude, double altitude) {}
    void setAppAdTracking(bool enable) {}
    void setPayingUser(bool isPayingUser) {}
    void setPreloadData(const TunePreloadData &preloadData) {}
    void setPreloadDataForScript(const std::string &jsonString) {}
    std::string matId() { return ""; }
    std::string tuneId() { return ""; }
    std::string openLogId() { return ""; }
    bool isPayingUser() { return false; }
    void measureSession() {}
    void measureEventName(const std::string &eventName) {}
    void measureEventId(int eventId) {}
    void measureEvent(const TuneEvent &event) {}
    void measureEventForScript(const std::string &jsonString) {}
    void setUseCookieTracking(bool enable) {}
    void setRedirectUrl(const std::string &redirectUrl) {}
    void startAppToAppTracking(const std::string &targetAppPackageName,
                               const std::string &targetAppAdvertiserId,
                               const std::string &targetAdvertiserOfferId,
                               const std::string &targetAdvertiserPublisherId,
                               bool shouldRedirect) {}
    void applicationDidOpenURL(const std::string &urlString, const std::string &sourceApplication) {}
    void setDeepLink(const std::string &deepLinkUrl) {}
};

class TuneWrapperEnabled : public TuneWrapper {
public:

    TuneWrapperEnabled();
    
    void setListener(TuneListener* listener);
    TuneListener* getListener();
    void removeListener();
    void init();
    void setDebugMode(bool enable);
    void setAllowDuplicateRequests(bool allow);
    void checkForDeferredDeeplinkWithTimeout(double timeout);
    void checkForDeferredDeepLink();
    void automateIapEventMeasurement(bool automate);
    void setFacebookEventLogging(bool logging, bool limit);
    void setExistingUser(bool existingUser);
    void setAppleAdvertisingIdentifier(const std::string &appleAdvertisingIdentifier, bool adTrackingEnabled);
    void setAppleVendorIdentifier(const std::string &appleVendorIdentifier);
    void setCurrencyCode(const std::string &currencyCode);
    void setJailbroken(bool jailbroken);
    void setPackageName(const std::string &packageName);
    void setShouldAutoDetectJailbroken(bool autoDetect);
    void setShouldAutoGenerateAppleVendorIdentifier(bool autoGenerate);
    void setSiteId(const std::string &siteId);
    void setTRUSTeId(const std::string &tpid);
    void setUserEmail(const std::string &userEmail);
    void setUserId(const std::string &userId);
    void setUserName(const std::string &userName);
    void setPhoneNumber(const std::string &phoneNumber);
    void setFacebookUserId(const std::string &facebookUserId);
    void setTwitterUserId(const std::string &twitterUserId);
    void setGoogleUserId(const std::string &googleUserId);
    void setAge(int userAge);
    void setGender(PluginTune::Gender userGender);
    void setLatitude(double latitude, double longitude);
    void setLatitude(double latitude, double longitude, double altitude);
    void setAppAdTracking(bool enable);
    void setPayingUser(bool isPayingUser);
    void setPreloadData(const TunePreloadData &preloadData);
    void setPreloadDataForScript(const std::string &jsonString);
    std::string matId();
    std::string tuneId();
    std::string openLogId();
    bool isPayingUser();
    void measureSession();
    void measureEventName(const std::string &eventName);
    void measureEventId(int eventId);
    void measureEvent(const TuneEvent &event);
    void measureEventForScript(const std::string &jsonString);
    void setUseCookieTracking(bool enable);
    void setRedirectUrl(const std::string &redirectUrl);
    void startAppToAppTracking(const std::string &targetAppPackageName,
                               const std::string &targetAppAdvertiserId,
                               const std::string &targetAdvertiserOfferId,
                               const std::string &targetAdvertiserPublisherId,
                               bool shouldRedirect);
    void applicationDidOpenURL(const std::string &urlString, const std::string &sourceApplication);
    void setDeepLink(const std::string &deepLinkUrl);
    
protected:

    bool nativeInit(const std::string& advertiserID, const std::string& conversionKey, bool debug);
    const char* nativeSDKVersion();

    bool _initialized;
    TuneListener* _listener;
};

NS_COCOS_END

#endif
