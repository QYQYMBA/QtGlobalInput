#ifndef QTGLOBALINPUT_H
#define QTGLOBALINPUT_H

#include <QVector>
#include <QKeyEvent>
#include <windows.h>
#include <functional>

#include "globalinputfilter.h"
#include "windowswitch.h"

class QtGlobalInput
{
public:
    enum class EventType
    {
        All,
        ButtonDown,
        ButtonUp
    };

    struct KeyHook{
        uint id;
        uint vkCode;
        EventType type;
        std::function<void(RAWKEYBOARD)> callback;
        bool async;
    };

    struct MouseHook{
        uint id;
        uint vkCode;
        EventType type;
        std::function<void(RAWMOUSE)> callback;
        bool async;
    };

    static void init(HWND hwnd);
    static void deinitialize();

    template<typename A, typename B>
    static uint setKeyPress(uint vkCode, EventType type, A callback, B obj, bool async = false);

    template<typename A, typename B>
    static uint setMousePress(uint vkCode, EventType type, A callback, B obj, bool async = false);

    template<typename A, typename B>
    static uint setWindowSwitch(A callback, B obj);

    static bool removeKeyPress(uint id);
    static bool removeMousePress(uint id);
    static bool removeWindowSwitch(uint id);

    static void newInput(RAWINPUT raw); //Function to be called from NativeEventFilter

private:
    QtGlobalInput();

    static void installNativeEventFilter();
    static void removeNativeEventFilter();

    static bool registerMouseInput();
    static bool removeMouseInput();

    static bool registerKeyboardInput();
    static bool removeKeyboardInput();

    static GlobalInputFilter _globalInputFilter;

    static QVector<MouseHook> _mouseHooks;
    static QVector<KeyHook> _keyHooks;

    static HWND _hwnd;

    static uint _id;

    static bool _nativeFilterInstalled;
};

template<typename A, typename B>
uint QtGlobalInput::setKeyPress(uint vkCode, EventType type, A callback, B obj, bool async)
{
    KeyHook kh;
    kh.callback = std::bind(callback, obj, std::placeholders::_1);
    kh.vkCode = vkCode;
    kh.type = type;
    kh.id = _id++;
    kh.async = async;

    _keyHooks.push_back(kh);

    if(!_nativeFilterInstalled)
    {
        installNativeEventFilter();
    }

    return kh.id;
}

template<typename A, typename B>
uint QtGlobalInput::setMousePress(uint vkCode, EventType type, A callback, B obj, bool async)
{
    MouseHook mh;
    mh.callback = std::bind(callback, obj, std::placeholders::_1);
    mh.vkCode = vkCode;
    mh.type = type;
    mh.id = _id++;
    mh.async = async;

    _mouseHooks.push_back(mh);

    if(!_nativeFilterInstalled)
    {
        installNativeEventFilter();
    }

    return mh.id;
}

template<typename A, typename B>
uint QtGlobalInput::setWindowSwitch(A callback, B obj)
{
    return WindowSwitch::setWindowSwitch(callback, obj);
}

#endif // QTGLOBALINPUT_H
