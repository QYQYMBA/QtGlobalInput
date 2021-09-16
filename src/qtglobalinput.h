#ifndef QTGLOBALINPUT_H
#define QTGLOBALINPUT_H

#include <QVector>
#include <QKeyEvent>
#include <windows.h>
#include <functional>

#include "globalinputfilter.h"

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

    template<typename A, typename B>
    uint setKeyPress(uint vkCode, EventType type, A callback, B obj);

    template<typename A, typename B>
    uint setMousePress(uint vkCode, EventType type, A callback, B obj);

    bool removeKeyPress(uint id);
    bool removeMousePress(uint id);

    void newInput(RAWINPUT raw); //Function to be called from NativeEventFilter
private:
    struct KeyHook{
        uint id;
        uint vkCode;
        EventType type;
        std::function<void(RAWKEYBOARD)> callback;
    };

    struct MouseHook{
        uint id;
        uint vkCode;
        EventType type;
        std::function<void(RAWMOUSE)> callback;
    };

    void installNativeEventFilter();
    void removeNativeEventFilter();

    bool registerMouseInput();
    bool removeMouseInput();

    bool registerKeyboardInput();
    bool removeKeyboardInput();

    GlobalInputFilter _globalInputFilter;

    QVector<MouseHook> _mouseHooks;
    QVector<KeyHook> _keyHooks;

    HWND _hwnd;

    uint _id;

    bool _nativeFilterInstalled;
};

#endif // QTGLOBALINPUT_H
