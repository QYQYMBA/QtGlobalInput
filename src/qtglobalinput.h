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

    QtGlobalInput(HWND hwnd);
    ~QtGlobalInput();

    template<typename A, typename B>
    uint setKeyPress(uint vkCode, EventType type, A callback, B obj, bool async = false);

    template<typename A, typename B>
    uint setMousePress(uint vkCode, EventType type, A callback, B obj, bool async = false);

    template<typename A, typename B>
    uint setWindowSwitch(A callback, B obj);

    bool removeKeyPress(uint id);
    bool removeMousePress(uint id);
    bool removeWindowSwitch(uint id);

    void newInput(RAWINPUT raw); //Function to be called from NativeEventFilter

private:
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

    void installNativeEventFilter();
    void removeNativeEventFilter();

    bool registerMouseInput();
    bool removeMouseInput();

    bool registerKeyboardInput();
    bool removeKeyboardInput();

    GlobalInputFilter _globalInputFilter;
    WindowSwitch _windowSwitch;

    QVector<MouseHook> _mouseHooks;
    QVector<KeyHook> _keyHooks;

    HWND _hwnd;

    uint _id;

    bool _nativeFilterInstalled;
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
    return _windowSwitch.setWindowSwitch(callback, obj);
}

#endif // QTGLOBALINPUT_H
