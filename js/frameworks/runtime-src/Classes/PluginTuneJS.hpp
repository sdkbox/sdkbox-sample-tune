#ifndef __PluginTuneJS_h__
#define __PluginTuneJS_h__

#include "jsapi.h"
#include "jsfriendapi.h"


#if MOZJS_MAJOR_VERSION >= 33
void js_register_PluginTuneJS_PluginTune(JSContext *cx, JS::HandleObject global);
void register_all_PluginTuneJS(JSContext* cx, JS::HandleObject obj);
#else
void js_register_PluginTuneJS_PluginTune(JSContext *cx, JSObject* global);
void register_all_PluginTuneJS(JSContext* cx, JSObject* obj);
#endif
#endif

