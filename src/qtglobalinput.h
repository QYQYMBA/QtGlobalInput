#ifndef QTGLOBALINPUT_H
#define QTGLOBALINPUT_H

#include <windows.h>
#include <functional>

#include "windowswitch.h"
#include "keyboardhooks.h"
#include "structures.h"
#include "rawinput.h"

class QtGlobalInput
{
public:
    static void init(HWND hwnd);
    static void deinitialize();

    template<typename A, typename B>
    static uint setLlKeyboardHook(uint vkCode, EventType type, A callback, B obj, bool async = false);
    static bool removeLlKeyboardHook(uint id);

    template<typename A, typename B>
    static uint waitForKeyPress(uint vkCode, EventType type, A callback, B obj, bool async = false);
    static bool removeKeyPress(uint id);

    template<typename A, typename B>
    static uint waitForMousePress(uint vkCode, EventType type, A callback, B obj, bool async = false);
    static bool removeMousePress(uint id);

    template<typename A, typename B>
    static uint setWindowSwitch(A callback, B obj);
    static bool removeWindowSwitch(uint id);

private:
    QtGlobalInput();

    static HWND _hwnd;
};

template<typename A, typename B>
uint QtGlobalInput::setLlKeyboardHook(uint vkCode, EventType type, A callback, B obj, bool async)
{
    return llKeyboardHooks::setLlHook(vkCode, type, callback, obj, async);
}

template<typename A, typename B>
uint QtGlobalInput::waitForKeyPress(uint vkCode, EventType type, A callback, B obj, bool async)
{
    return RawInput::waitForKeyPress(vkCode, type, callback, obj, async);
}

template<typename A, typename B>
uint QtGlobalInput::waitForMousePress(uint vkCode, EventType type, A callback, B obj, bool async)
{
    return RawInput::waitForMousePress(vkCode, type, callback, obj, async);
}

template<typename A, typename B>
uint QtGlobalInput::setWindowSwitch(A callback, B obj)
{
    return WindowSwitch::setWindowSwitch(callback, obj);
}

#endif // QTGLOBALINPUT_H
