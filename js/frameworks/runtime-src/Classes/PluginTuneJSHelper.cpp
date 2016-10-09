
#include "PluginTuneJSHelper.h"
#include "PluginTuneJS.hpp"
#include "PluginTune/PluginTune.h"
#include <sstream>
#include "SDKBoxJSHelper.h"

extern JSObject* jsb_sdkbox_PluginTune_prototype;

static JSContext* s_cx = nullptr;

USING_NS_CC;

#if (COCOS2D_VERSION < 0x00030000)
#define Ref CCObject
#define Director CCDirector
#define getInstance sharedDirector
#define schedule scheduleSelector
#endif

class JsCallbackObj : public Ref
{
public:
    static JsCallbackObj *create(const std::string &eventName, const std::string &eventData, bool timeout, JSObject *handler)
    {
        JsCallbackObj *obj = new JsCallbackObj(eventName, eventData, timeout, handler);
        obj->autorelease();
        return obj;
    }

    void start()
    {
        Director::getInstance()->getScheduler()->schedule(schedule_selector(JsCallbackObj::callback), this, 0.1, 0, 0.0f, false);
    }

    void callback(float dt)
    {
        if (!s_cx)
        {
            return;
        }
        JSContext* cx = s_cx;
        const char* func_name = _eventName.data();

        JS::RootedObject obj(cx, _jsHandler);
        JSAutoCompartment ac(cx, obj);

#if defined(MOZJS_MAJOR_VERSION)
#if MOZJS_MAJOR_VERSION >= 33
        bool hasAction;
        JS::RootedValue retval(cx);
        JS::RootedValue func_handle(cx);
#else
        bool hasAction;
        jsval retval;
        JS::RootedValue func_handle(cx);
#endif
#elif defined(JS_VERSION)
        JSBool hasAction;
        jsval retval;
        jsval func_handle;
#endif

        jsval dataVal[2];
        dataVal[0] = c_string_to_jsval(cx, _eventData.c_str());
        dataVal[1] = BOOLEAN_TO_JSVAL(_timeout);
        int paramsCount = 1;
        if ("onReceiveDeeplink" == _eventName) {
            paramsCount = 2;
        }

        if (JS_HasProperty(cx, obj, func_name, &hasAction) && hasAction) {
            if(!JS_GetProperty(cx, obj, func_name, &func_handle)) {
                return;
            }
            if(func_handle == JSVAL_VOID) {
                return;
            }

#if MOZJS_MAJOR_VERSION >= 31
            JS_CallFunctionName(cx, obj, func_name, JS::HandleValueArray::fromMarkedLocation(paramsCount, dataVal), &retval);
#else
            JS_CallFunctionName(cx, obj, func_name, paramsCount, dataVal, &retval);
#endif
        }
        release();
    }

private:
    JsCallbackObj(const std::string &eventName, const std::string &eventData, bool timeout, JSObject *handler)
    : _eventName(eventName)
    , _eventData(eventData)
    , _timeout(timeout)
    , _jsHandler(handler)
    {
        retain();
    }

    JSObject* _jsHandler;
    std::string _eventName;
    std::string _eventData;
    bool _timeout;
}; // JsCallbackObj


class TuneListenerJs : public sdkbox::TuneListener, public sdkbox::JSListenerBase
{
public:
    TuneListenerJs():sdkbox::JSListenerBase() {
    }

    virtual void onMobileAppTrackerEnqueuedActionWithReferenceId(const std::string &referenceId)
    {
        JsCallbackObj::create("onEnqueuedAction", referenceId, false, getJSDelegate())->start();
    }
    virtual void onMobileAppTrackerDidSucceedWithData(const std::string &data)
    {
        JsCallbackObj::create("onSucceed", data, false, getJSDelegate())->start();
    }
    virtual void onMobileAppTrackerDidFailWithError(const std::string &errorString)
    {
        JsCallbackObj::create("onFailed", errorString, false, getJSDelegate())->start();
    }
    virtual void onMobileAppTrackerDidReceiveDeeplink(const std::string &deeplink, bool timeout)
    {
        JsCallbackObj::create("onReceiveDeeplink", deeplink, timeout, getJSDelegate())->start();
    }
    virtual void onMobileAppTrackerDidFailDeeplinkWithError(const std::string &errorString)
    {
        JsCallbackObj::create("onFailDeeplink", errorString, false, getJSDelegate())->start();
    }

}; // TuneListenerJs

#if defined(MOZJS_MAJOR_VERSION)
bool js_PluginTuneJS_PluginTune_setListener(JSContext *cx, uint32_t argc, jsval *vp)
#elif defined(JS_VERSION)
JSBool js_PluginTuneJS_PluginTune_setListener(JSContext *cx, unsigned argc, JS::Value *vp)
#endif
{
    s_cx = cx;
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;

    if (argc == 1) {

        if (!args.get(0).isObject())
        {
            ok = false;
        }

        JSB_PRECONDITION2(ok, cx, false, "js_PluginTuneJS_PluginTune_setListener : Error processing arguments");
        TuneListenerJs *lis = new TuneListenerJs();
        lis->setJSDelegate(args.get(0));
        sdkbox::PluginTune::setListener(lis);

        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_PluginTuneJS_PluginTune_setListener : wrong number of arguments");
    return false;
}

#if defined(MOZJS_MAJOR_VERSION)
bool js_PluginTuneJS_PluginTune_setLatitude(JSContext *cx, uint32_t argc, jsval *vp)
#elif defined(JS_VERSION)
JSBool js_PluginTuneJS_PluginTune_setLatitude(JSContext *cx, uint32_t argc, jsval *vp)
#endif
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;

    do {
        if (argc == 3) {
            double arg0;
            ok &= sdkbox::js_to_number(cx, args.get(0), &arg0);
            if (!ok) { ok = true; break; }
            double arg1;
            ok &= sdkbox::js_to_number(cx, args.get(1), &arg1);
            if (!ok) { ok = true; break; }
            double arg2;
            ok &= sdkbox::js_to_number(cx, args.get(2), &arg2);
            if (!ok) { ok = true; break; }
            sdkbox::PluginTune::setLatitude(arg0, arg1, arg2);
            return true;
        }
    } while (0);

    do {
        if (argc == 2) {
            double arg0;
            ok &= sdkbox::js_to_number(cx, args.get(0), &arg0);
            if (!ok) { ok = true; break; }
            double arg1;
            ok &= sdkbox::js_to_number(cx, args.get(1), &arg1);
            if (!ok) { ok = true; break; }
            sdkbox::PluginTune::setLatitude(arg0, arg1);
            return true;
        }
    } while (0);
    JS_ReportError(cx, "js_PluginTuneJS_PluginTune_setLatitude : wrong number of arguments");
    return false;
}

#define TUNE_LOCAL_REGISTER \
JS_DefineFunction(cx, pluginObj, "setListener", js_PluginTuneJS_PluginTune_setListener, 1, JSPROP_READONLY | JSPROP_PERMANENT); \
JS_DefineFunction(cx, pluginObj, "setLatitude", js_PluginTuneJS_PluginTune_setLatitude, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE); \
JS_DefineFunction(cx, pluginObj, "measureEvent", js_PluginTuneJS_PluginTune_measureEventForScript, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE); \
JS_DefineFunction(cx, pluginObj, "setPreloadData", js_PluginTuneJS_PluginTune_setPreloadDataForScript, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE);

#if defined(MOZJS_MAJOR_VERSION)
#if MOZJS_MAJOR_VERSION >= 33
void register_all_PluginTuneJS_helper(JSContext* cx, JS::HandleObject global) {
    JS::RootedObject pluginObj(cx);
    sdkbox::getJsObjOrCreat(cx, global, "sdkbox.PluginTune", &pluginObj);

    TUNE_LOCAL_REGISTER
}
#else
void register_all_PluginTuneJS_helper(JSContext* cx, JSObject* global) {
    JS::RootedObject pluginObj(cx);
    sdkbox::getJsObjOrCreat(cx, JS::RootedObject(cx, global), "sdkbox.PluginTune", &pluginObj);

    TUNE_LOCAL_REGISTER
}
#endif
#elif defined(JS_VERSION)
void register_all_PluginTuneJS_helper(JSContext* cx, JSObject* global) {
    jsval pluginVal;
    JSObject* pluginObj;
    pluginVal = sdkbox::getJsObjOrCreat(cx, global, "sdkbox.PluginTune", &pluginObj);

    TUNE_LOCAL_REGISTER
}
#endif
