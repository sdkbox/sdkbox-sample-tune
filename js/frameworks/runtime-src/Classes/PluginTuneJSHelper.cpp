
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

class TuneCallbackJS: public cocos2d::Ref {
public:
    TuneCallbackJS();
    void schedule();
    void notityJs(float dt);

    std::string _name;

    JS::Value _paramVal[2];
    int _paramLen;
};

class TuneListenerJS : public sdkbox::TuneListener, public sdkbox::JSListenerBase
{
public:
    TuneListenerJS():sdkbox::JSListenerBase() {
    }

    void onMobileAppTrackerEnqueuedActionWithReferenceId(const std::string &referenceId)
    {
        TuneCallbackJS* cb = new TuneCallbackJS();
#if MOZJS_MAJOR_VERSION < 52
        JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
#endif
        cb->_name = "onEnqueuedAction";
        cb->_paramVal[0] = SB_STR_TO_JSVAL(s_cx, referenceId);
        cb->_paramLen = 1;
        cb->schedule();
    }
    void onMobileAppTrackerDidSucceedWithData(const std::string &data)
    {
        TuneCallbackJS* cb = new TuneCallbackJS();
#if MOZJS_MAJOR_VERSION < 52
        JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
#endif
        cb->_name = "onSucceed";
        cb->_paramVal[0] = SB_STR_TO_JSVAL(s_cx, data);
        cb->_paramLen = 1;
        cb->schedule();
    }
    void onMobileAppTrackerDidFailWithError(const std::string &errorString)
    {
        TuneCallbackJS* cb = new TuneCallbackJS();
#if MOZJS_MAJOR_VERSION < 52
        JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
#endif
        cb->_name = "onFailed";
        cb->_paramVal[0] = SB_STR_TO_JSVAL(s_cx, errorString);
        cb->_paramLen = 1;
        cb->schedule();
    }
    void onMobileAppTrackerDidReceiveDeeplink(const std::string &deeplink, bool timeout)
    {
        TuneCallbackJS* cb = new TuneCallbackJS();
#if MOZJS_MAJOR_VERSION < 52
        JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
#endif
        cb->_name = "onFailed";
        cb->_paramVal[0] = SB_STR_TO_JSVAL(s_cx, deeplink);
        cb->_paramVal[1] = JS::BooleanValue(timeout);
        cb->_paramLen = 2;
        cb->schedule();
    }
    void onMobileAppTrackerDidFailDeeplinkWithError(const std::string &errorString)
    {
        TuneCallbackJS* cb = new TuneCallbackJS();
#if MOZJS_MAJOR_VERSION < 52
        JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
#endif
        cb->_name = "onFailDeeplink";
        cb->_paramVal[0] = SB_STR_TO_JSVAL(s_cx, errorString);
        cb->_paramLen = 1;
        cb->schedule();
    }

    void invokeJS(const char* func, JS::Value *pVals, int valueSize)
    {
        if (!s_cx) {
            return;
        }
        JSContext* cx = s_cx;
        const char* func_name = func;
        JS::RootedObject obj(cx, getJSDelegate());
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

        if (JS_HasProperty(cx, obj, func_name, &hasAction) && hasAction) {
            if(!JS_GetProperty(cx, obj, func_name, &func_handle)) {
                return;
            }
            if(func_handle == JS::NullValue()) {
                return;
            }

#if MOZJS_MAJOR_VERSION >= 31
            if (0 == valueSize) {
                JS_CallFunctionName(cx, obj, func_name, JS::HandleValueArray::empty(), &retval);
            } else {
                JS_CallFunctionName(cx, obj, func_name, JS::HandleValueArray::fromMarkedLocation(valueSize, pVals), &retval);
            }
#else
            if (0 == valueSize) {
                JS_CallFunctionName(cx, obj, func_name, 0, nullptr, &retval);
            } else {
                JS_CallFunctionName(cx, obj, func_name, valueSize, pVals, &retval);
            }
#endif
        }
    }

};


TuneCallbackJS::TuneCallbackJS():
_paramLen(0) {
}

void TuneCallbackJS::schedule() {
    retain();
    cocos2d::Director::getInstance()->getScheduler()->schedule(schedule_selector(TuneCallbackJS::notityJs), this, 0, 0, 0.0f, false);
    autorelease();
}

void TuneCallbackJS::notityJs(float dt) {
    sdkbox::TuneListener* lis = sdkbox::PluginTune::getListener();
    TuneListenerJS* l = dynamic_cast<TuneListenerJS*>(lis);
    if (l) {
        l->invokeJS(_name.c_str(), _paramVal, _paramLen);
    }
    release();
}

#if defined(MOZJS_MAJOR_VERSION)
bool js_PluginTuneJS_PluginTune_setListener(JSContext *cx, uint32_t argc, JS::Value *vp)
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
        TuneListenerJS *lis = new TuneListenerJS();
        lis->setJSDelegate(cx, args.get(0));
        sdkbox::PluginTune::setListener(lis);

        args.rval().setUndefined();
        return true;
    }
    JS_ReportErrorUTF8(cx, "js_PluginTuneJS_PluginTune_setListener : wrong number of arguments");
    return false;
}

#if defined(MOZJS_MAJOR_VERSION)
bool js_PluginTuneJS_PluginTune_setLatitude(JSContext *cx, uint32_t argc, JS::Value *vp)
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
    JS_ReportErrorUTF8(cx, "js_PluginTuneJS_PluginTune_setLatitude : wrong number of arguments");
    return false;
}

#define TUNE_LOCAL_REGISTER \
JS_DefineFunction(cx, pluginObj, "setListener", js_PluginTuneJS_PluginTune_setListener, 1, JSPROP_READONLY | JSPROP_PERMANENT); \
JS_DefineFunction(cx, pluginObj, "setLatitude", js_PluginTuneJS_PluginTune_setLatitude, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE);

/*
JS_DefineFunction(cx, pluginObj, "measureEvent", js_PluginTuneJS_PluginTune_measureEventForScript, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE); \
JS_DefineFunction(cx, pluginObj, "setPreloadData", js_PluginTuneJS_PluginTune_setPreloadDataForScript, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE);
 */

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
