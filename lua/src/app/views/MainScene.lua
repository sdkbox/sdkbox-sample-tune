
local MainScene = class("MainScene", cc.load("mvc").ViewBase)

function MainScene:onCreate()
    print("Sample Startup")

    local label = cc.Label:createWithSystemFont("QUIT", "sans", 32)
    local quit = cc.MenuItemLabel:create(label)
    quit:onClicked(function()
        os.exit(0)
    end)
    local size = label:getContentSize()
    local menu = cc.Menu:create(quit)
    menu:setPosition(display.right - size.width / 2 - 16, display.bottom + size.height / 2 + 16)
    self:addChild(menu)

    self:setupTestMenu()
end

function MainScene:setupTestMenu()
    sdkbox.PluginTune:setDebugMode(true)
    sdkbox.PluginTune:measureSession()
    sdkbox.PluginTune:setListener(function (args)
        dump(args)
    end)

    sdkbox.PluginTune:checkForDeferredDeepLink()

    local label1 = cc.Label:createWithSystemFont("purchase event", "sans", 28)
    local item1 = cc.MenuItemLabel:create(label1)
    item1:onClicked(function()
        print("purchase event")
        sdkbox.PluginTune:measureEventName("purchase")
        sdkbox.PluginTune:measureEventId(1122334455)
        local event = {}
        event.eventName = "purchase2"
        event.refId     = "RJ1357"
        event.searchString = "sweet crisp red apples"
        event.attribute1 = "crisp"
        event.attribute2 = "red"
        event.quantity = 3
        sdkbox.PluginTune:measureEvent(json.encode(event))
    end)

    local menu = cc.Menu:create(item1)
    menu:alignItemsVerticallyWithPadding(24)
    self:addChild(menu)
end

return MainScene
