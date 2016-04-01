
var HelloWorldLayer = cc.Layer.extend({
    sprite:null,
    ctor:function () {
        //////////////////////////////
        // 1. super init first
        this._super();

        cc.log("Sample Startup")

        this.createTestMenu();

        var winsize = cc.winSize;

        var logo = new cc.Sprite("res/Logo.png");
        var logoSize = logo.getContentSize();
        logo.x = logoSize.width / 2;
        logo.y = winsize.height - logoSize.height / 2;
        this.addChild(logo);

        var quit = new cc.MenuItemLabel(new cc.LabelTTF("QUIT", "sans", 32), function() {
            cc.log("QUIT");
        });
        var menu = new cc.Menu(quit);
        var size = quit.getContentSize();
        menu.x = winsize.width - size.width / 2 - 16;
        menu.y = size.height / 2 + 16;
        this.addChild(menu);

        return true;
    },

    createTestMenu:function() {
        sdkbox.PluginTune.init();
        sdkbox.PluginTune.setDebugMode(true);
        sdkbox.PluginTune.measureSession();
        sdkbox.PluginTune.setListener({
            onEnqueuedAction: function(data) {
                cc.log("onEnqueuedAction");
            },
            onSucceed: function(data) {
                cc.log("onSucceed");
            },
            onFailed: function(data) {
                cc.log("onFailed");
            },
            onReceiveDeeplink: function(data, timeout) {
                cc.log("onReceiveDeeplink");
            },
            onFailDeeplink: function(errorString) {
                cc.log("onFailDeeplink");
            }
        });

        sdkbox.PluginTune.checkForDeferredDeepLink();


        var item1 = new cc.MenuItemLabel(new cc.LabelTTF("purchase event", "sans", 28), function() {
            cc.log("purchase event");

            sdkbox.PluginTune.measureEventName("login-2016-04-01");
            sdkbox.PluginTune.measureEventId(0123456789);

            var event = {};
            event.eventName = "purchase";
            event.refId = "RJ1357";
            event.searchString = "sweet srisp red apples";
            event.attribute1 = "srisp";
            event.attribute2 = "red";
            event.quantity = 3;
            sdkbox.PluginTune.measureEvent(JSON.stringify(event));
        });

        var winsize = cc.winSize;
        var menu = new cc.Menu(item1);
        menu.x = winsize.width / 2;
        menu.y = winsize.height / 2;
        menu.alignItemsVerticallyWithPadding(20);
        this.addChild(menu);
    }
});

var HelloWorldScene = cc.Scene.extend({
    onEnter:function () {
        this._super();
        var layer = new HelloWorldLayer();
        this.addChild(layer);
    }
});

