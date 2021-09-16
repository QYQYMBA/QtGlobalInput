#include "qtglobalinput.h"

template<typename A, typename B>
uint QtGlobalInput::setKeyPress(uint vkCode, EventType type, A callback, B obj)
{
    KeyHook kh;
    kh.callback = std::bind(callback, obj);
    kh.vkCode = vkCode;
    kh.type = type;
    kh.id = _id++;

    _keyHooks.push_back(kh);

    if(!_nativeFilterInstalled)
    {
        installNativeEventFilter();
    }

    return kh.id;
}

template<typename A, typename B>
uint QtGlobalInput::setMousePress(uint vkCode, EventType type, A callback, B obj)
{
    MouseHook mh;
    mh.callback = std::bind(callback, obj);
    mh.vkCode = vkCode;
    mh.type = type;
    mh.id = _id++;

    _mouseHooks.push_back(mh);

    if(!_nativeFilterInstalled)
    {
        installNativeEventFilter();
    }

    return mh.id;
}
