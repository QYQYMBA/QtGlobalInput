#ifndef RAWINPUT_H
#define RAWINPUT_H

#include <QVector>
#include <QKeyEvent>
#include <windows.h>
#include <functional>

#include "structures.h"
#include "globalinputfilter.h"

class RawInput
{
public:
    static void init(HWND hwnd);
    static void deinitialize();

    template<typename A, typename B>
    static uint waitForKeyPress(uint vkCode, EventType type, A callback, B obj, bool async = false);
    static bool removeKeyPress(uint id);

    template<typename A, typename B>
    static uint waitForMousePress(uint vkCode, EventType type, A callback, B obj, bool async = false);
    static bool removeMousePress(uint id);

    static void newInput(RAWINPUT raw); //Function to be called from NativeEventFilter

private:
    RawInput() {}

    static void installNativeEventFilter();
    static void removeNativeEventFilter();

    static bool registerMouseInput();
    static bool removeMouseInput();

    static bool registerKeyboardInput();
    static bool removeKeyboardInput();


    static GlobalInputFilter _globalInputFilter;

    static QVector<MouseHook> _mouseHooks;
    static QVector<KeyHook> _keyHooks;

    static uint _id;

    static bool _nativeFilterInstalled;

    static HWND _hwnd;
};

template<typename A, typename B>
uint RawInput::waitForKeyPress(uint vkCode, EventType type, A callback, B obj, bool async)
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
uint RawInput::waitForMousePress(uint vkCode, EventType type, A callback, B obj, bool async)
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

#endif // RAWINPUT_H
